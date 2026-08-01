#!/usr/bin/env python3
"""Extract per-case-body parameters from a retail mpvabdec_*_Isr listing.

Output: a dict body_offset -> params, dumped as python source (body_params.py).
The analysis is per-function; the three functions share structure but differ
in register allocation and quant multipliers.
"""
import json
import re
import sys

RE_LINE = re.compile(
    r'/\* ([0-9A-Fa-f]+) [0-9A-Fa-f]+  ([0-9A-Fa-f ]+) \*/\s+(.*)')


def parse(path, fn=None):
    out = []
    base = None
    started = fn is None
    for l in open(path):
        if fn is not None:
            if '.fn %s, global' % fn in l:
                started = True
                continue
            if started and ('.endfn' in l or l.startswith('# .text:') and started and out):
                break
        m = RE_LINE.match(l)
        if m and started:
            addr = int(m.group(1), 16)
            if base is None:
                base = addr
            out.append((addr - base, m.group(2).replace(' ', ''), m.group(3)))
    return out


def find_dispatch(asm):
    for i, (off, b, ins) in enumerate(asm):
        if 'bctr' in ins:
            return off, i
    raise ValueError("no dispatch")


def body_ranges(asm, jt, base):
    """Return ordered list of (rel_offset, (start_idx, end_idx)) for each
    distinct jump-table target, and the set of indices per target."""
    from collections import defaultdict
    groups = defaultdict(list)
    for idx, t in enumerate(jt):
        groups[(t & ~0x3) - base].append(idx)
    targets = sorted(groups)
    idx_of = {off: i for i, (off, b, ins) in enumerate(asm)}
    ranges = {}
    for i, t in enumerate(targets):
        end = targets[i + 1] if i + 1 < len(targets) else asm[-1][0] + 4
        ranges[t] = (idx_of[t], idx_of.get(end, len(asm)))
    return targets, ranges, groups


def analyze_body(asm, rng):
    """Classify a body. Returns a dict of parameters."""
    p = {}
    insns = [asm[i][2] for i in range(rng[0], rng[1])]
    text = '\n'.join(insns)

    # coefficient loads: first lbz/lbzu referencing the p register
    # (we detect by displacement; p is the base with disp != 0 first)
    loads = []
    for ins in insns:
        m = re.match(r'lbz(u)? r(\d+), (0x[0-9a-fA-F]+)\(r(\d+)\)', ins)
        if m:
            loads.append((m.group(1) == 'u', int(m.group(3), 16), m.group(2)))
    if not loads:
        # eob / escape / huff bodies have no coefficient load
        if 'tbl990' in text or '0x990(r' in text:
            p['special'] = 'huff990'
        elif '0x9a8(r' in text or '0x9a4(r' in text:
            p['special'] = 'escape_long'
        elif '0x998(r' in text or '0x994(r' in text:
            p['special'] = 'huff'
        elif re.search(r'addi r\d+, r\d+, 0x2', text) and 'slwi r0, r0, 2' in text:
            p['special'] = 'eob'
        else:
            p['special'] = 'unknown'
        return p

    # number of coefficients: count distinct load groups separated by pointer
    # update. First load without 'u' then with 'u' => 2 coeffs.
    if loads[0][0] is False and len(loads) > 1 and loads[1][0] is True:
        p['coeffs'] = 2
        p['off'] = loads[0][1]
    else:
        p['coeffs'] = 1
        p['off'] = loads[0][1]

    # neg usage (count 'neg' instructions)
    p['neg'] = text.count('neg r')

    # multiplier: find slwi/mulli and subf/add after them
    m = re.search(r'mulli r\d+, r\d+, (0x[0-9a-fA-F]+)', text)
    if m:
        p['mult'] = ('mulli', int(m.group(1), 16))
    else:
        sl = re.search(r'slwi r(\d+), r(\d+), ([123])', text)
        if sl:
            base = 2 ** int(sl.group(3))
            d = sl.group(1)   # slwi dest
            q = sl.group(2)   # source (quant)
            # (2^N - 1)*q: subf rX, rQ, rD   (rD - rQ)
            m2 = re.search(r'subf r\d+, r' + q + r', r' + d, text)
            if m2:
                p['mult'] = base - 1
            else:
                # (2^N + 1)*q: add rX, rD, rQ
                m2 = re.search(r'add r\d+, r' + d + r', r' + q, text)
                if m2:
                    p['mult'] = base + 1
                else:
                    p['mult'] = base
        else:
            p['mult'] = None

    # bits: the addi to the bc register after the first load
    # find "addi rX, rX, K" where the first operand equals the bc reg
    # (bc reg = the one used in cmpwi rX, 0x20)
    bc = None
    for ins in insns:
        m2 = re.search(r'cmpwi r(\d+), 0x20', ins)
        if m2:
            bc = m2.group(1)
            break
    bits = None
    if bc is not None:
        for ins in insns:
            m2 = re.match(r'addi r' + bc + r', r' + bc + r', (0x[0-9a-fA-F]+)', ins)
            if m2:
                bits = int(m2.group(1), 16)
                break
        if bits is None:
            m2 = re.match(r'add r' + bc + r', r' + bc + r', r(\d+)', text)
            if m2:
                bits = 'codelen'
    p['bits'] = bits

    # exit: look at last unconditional branch
    last_ins = insns[-1] if insns else ''
    if not re.match(r'b ', last_ins):
        p['exit'] = 'fall'
    else:
        m2 = re.search(r'\.L_803B([0-9A-Fa-f]+)', last_ins)
        if m2:
            p['exit'] = 'target'
            p['exit_target'] = int(m2.group(1), 16)
        else:
            p['exit'] = 'other'
    return p


def main():
    path = sys.argv[1]
    jt_path = sys.argv[2]
    base = int(sys.argv[3], 16)
    out_path = sys.argv[4]
    fn = sys.argv[5] if len(sys.argv) > 5 else None
    asm = parse(path, fn)
    jt = json.load(open(jt_path))
    targets, ranges, groups = body_ranges(asm, jt, base)
    params = {}
    for t in targets:
        rng = ranges[t]
        p = analyze_body(asm, rng)
        p['indices'] = sorted(groups[t])
        params[t] = p
    # dump
    with open(out_path, 'w') as f:
        f.write("BODIES = {\n")
        for t in sorted(params):
            p = params[t]
            f.write(f"    0x{t:04X}: dict(")
            items = []
            if 'special' in p:
                items.append(f"special={p['special']!r}")
            else:
                items.append(f"coeffs={p['coeffs']}")
                items.append(f"off={p['off']}")
                items.append(f"neg={p['neg']}")
                items.append(f"mult={p['mult']!r}")
                items.append(f"bits={p['bits']!r}")
            items.append(f"idx={p['indices']}")
            if 'exit' in p and p['exit'] != 'target':
                items.append(f"exit={p['exit']!r}")
            elif 'exit' in p:
                items.append(f"exit=({p['exit']!r}, 0x{p['exit_target']:X})")
            f.write(', '.join(items))
            f.write("),\n")
        f.write("}\n")
    print(f"wrote {out_path}: {len(params)} bodies")


if __name__ == '__main__':
    main()
