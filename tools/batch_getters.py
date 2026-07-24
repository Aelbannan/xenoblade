#!/usr/bin/env python3
"""
Batch-decompile simple 0x4 (blr) and 0x8 (getter/setter/thunk/return-imm) functions
in catalog TUs with LLM-HARNESS markers.

Usage:
  python3 tools/batch_getters.py claim     # Claim all matching targets
  python3 tools/batch_getters.py edit      # Edit source files with proper C++ code
  python3 tools/batch_getters.py cycle     # Cycle all claimed targets (1 per TU first)
  python3 tools/batch_getters.py all       # claim + edit + cycle
  python3 tools/batch_getters.py status    # Show what's left to do
"""

import json, re, os, sys, subprocess, time

TARGETS_JSON = 'tools/coop/targets.json'

def find_src(unit):
    if unit.startswith('kyoshin/'):
        return f'src/{unit}.cpp'
    if unit.startswith(('monolib/', 'CriWare/', 'RVL_SDK/', 'PowerPC/')):
        return f'libs/{unit}.cpp'
    return None

def is_catalog_tu(src):
    """Check if source has LLM-HARNESS markers."""
    if not src or not os.path.exists(src):
        return False
    with open(src) as f:
        head = f.read(1000)
        return 'LLM-HARNESS' in head

def load_targets():
    with open(TARGETS_JSON) as f:
        return json.load(f)

def save_targets(data):
    with open(TARGETS_JSON, 'w') as f:
        json.dump(data, f, indent=2)

def get_asm_body(unit, sym):
    asm_path = f'build/us/asm/{unit}.s'
    if not os.path.exists(asm_path):
        return None
    with open(asm_path) as f:
        asm = f.read()
    m = re.search(r'\.fn ' + re.escape(sym) + r',.*?\n(.*?)\.endfn', asm, re.DOTALL)
    return m.group(1).strip() if m else None

def parse_instructions(body):
    """Extract instruction tuples (mnemonic, operands_str) from assembly body."""
    instrs = []
    for line in body.split('\n'):
        line = line.strip()
        if not line:
            continue
        # Strip comment /* ... */
        idx = line.find('*/')
        if idx >= 0:
            line = line[idx+2:].strip()
        parts = line.split()
        if parts:
            instrs.append((parts[0], ' '.join(parts[1:])))
    return instrs

def classify_0x8(instrs, sym, unit):
    """
    Classify a 0x8 function and generate C++ code template.
    Returns (ret_type, body_code) or None if unhandled.
    """
    if len(instrs) != 2:
        return None
    
    mnem1, ops1 = instrs[0]
    mnem2, ops2 = instrs[1]
    
    # ---- Pattern: li rD, imm + blr  (181x) ----
    if mnem1 == 'li' and mnem2 == 'blr':
        m = re.match(r'^r3,\s*(.+)$', ops1)
        if m:
            imm_str = m.group(1).strip()
            # Try to parse immediate
            try:
                imm = int(imm_str, 0)
                return ('int', f'return {imm};')
            except ValueError:
                return ('int', f'return ({imm_str});')
    
    # ---- Pattern: lbz r3, off(r3) + blr  (23x) ----
    if mnem1 == 'lbz' and mnem2 == 'blr':
        m = re.match(r'^r3,\s*(\d+|0x[0-9A-Fa-f]+)\(r3\)$', ops1)
        if m: return ('u8', f'return ((u8*)self)[{m.group(1)}];')
    
    # ---- Pattern: lhz r3, off(r3) + blr  (12x) ----
    if mnem1 == 'lhz' and mnem2 == 'blr':
        m = re.match(r'^r3,\s*(\d+|0x[0-9A-Fa-f]+)\(r3\)$', ops1)
        if m: return ('u16', f'return *(u16*)((u8*)self + {m.group(1)});')
    
    # ---- Pattern: lha r3, off(r3) + blr  (4x) ----
    if mnem1 == 'lha' and mnem2 == 'blr':
        m = re.match(r'^r3,\s*(\d+|0x[0-9A-Fa-f]+)\(r3\)$', ops1)
        if m: return ('s16', f'return *(s16*)((u8*)self + {m.group(1)});')
    
    # ---- Pattern: lwz r3, off(r3) + blr  (77x) ----
    if mnem1 == 'lwz' and mnem2 == 'blr':
        m = re.match(r'^r3,\s*(\d+|0x[0-9A-Fa-f]+)\(r3\)$', ops1)
        if m: return ('u32', f'return *(u32*)((u8*)self + {m.group(1)});')
    
    # ---- Pattern: lfs r1, off(r3) + blr  (51x) ----
    if mnem1 == 'lfs' and mnem2 == 'blr':
        m = re.match(r'^f1,\s*(\d+|0x[0-9A-Fa-f]+)\(r3\)$', ops1)
        if m: return ('float', f'return *(float*)((u8*)self + {m.group(1)});')
    
    # ---- Pattern: stb r4, off(r3) + blr  (5x) ----
    if mnem1 == 'stb' and mnem2 == 'blr':
        m = re.match(r'^r4,\s*(\d+|0x[0-9A-Fa-f]+)\(r3\)$', ops1)
        if m: return ('void', f'((u8*)self)[{m.group(1)}] = val;', 'u8', 'val')
    
    # ---- Pattern: stw r4, off(r3) + blr  (17x) ----
    if mnem1 == 'stw' and mnem2 == 'blr':
        m = re.match(r'^r4,\s*(\d+|0x[0-9A-Fa-f]+)\(r3\)$', ops1)
        if m: return ('void', f'*(u32*)((u8*)self + {m.group(1)}) = val;', 'u32', 'val')
    
    # ---- Pattern: stfs r1, off(r3) + blr  (50x) ----
    if mnem1 == 'stfs' and mnem2 == 'blr':
        m = re.match(r'^f1,\s*(\d+|0x[0-9A-Fa-f]+)\(r3\)$', ops1)
        if m: return ('void', f'*(float*)((u8*)self + {m.group(1)}) = val;', 'float', 'val')
    
    # ---- Pattern: addi r3, r3, off + blr  (85x) ----
    if mnem1 == 'addi' and mnem2 == 'blr':
        m = re.match(r'^r3,\s*r3,\s*(\d+|0x[0-9A-Fa-f]+)$', ops1)
        if m: return ('void*', f'return (void*)((u8*)self + {m.group(1)});')
    
    # ---- Pattern: li rD, imm + b target  (11x) ----
    if mnem1 == 'li' and mnem2 == 'b':
        # Tail call with immediate value set up
        # { return target(self + 0); } -- but we need the target address
        return ('thunk-li', ops1, ops2)
    
    # ---- Pattern: addi r3, r3, off + b target  (52x) ----
    if mnem1 == 'addi' and mnem2 == 'b':
        m = re.match(r'^r3,\s*r3,\s*(\d+|0x[0-9A-Fa-f]+)$', ops1)
        if m:
            offset = m.group(1)
            return ('thunk-addi', offset, ops2)
    
    # ---- Pattern: subi r3, r3, off + b target  (287x) ----
    if mnem1 == 'subi' and mnem2 == 'b':
        m = re.match(r'^r3,\s*r3,\s*(\d+|0x[0-9A-Fa-f]+)$', ops1)
        if m:
            offset = m.group(1)
            return ('thunk-subi', offset, ops2)
    
    # ---- Pattern: lwz r3, off(r3) + b target  (23x) ----
    if mnem1 == 'lwz' and mnem2 == 'b':
        m = re.match(r'^r3,\s*(\d+|0x[0-9A-Fa-f]+)\(r3\)$', ops1)
        if m:
            return ('thunk-load', m.group(1), ops2)
    
    return None

def find_targets(data):
    """Find all NOT_STARTED targets in catalog TUs and classify them."""
    results = []
    sym_to_id = {t['symbol']: t['id'] for t in data['targets']}
    
    for t in data['targets']:
        if t.get('status') != 'NOT_STARTED':
            continue
        unit = t.get('unit', '')
        src = find_src(unit)
        if not is_catalog_tu(src):
            continue
        if t.get('workflow_status') in ('COMPILING', 'ACTIVE', 'ACCEPTED', 'HIGH_MATCH', 'FULL_MATCH', 'EQUIVALENT_MATCH'):
            continue
        
        s = str(t.get('size','0x0'))
        size = int(s,16) if '0x' in s else 999
        tid = t['id']
        sym = t.get('symbol','')
        
        body = get_asm_body(unit, sym)
        if not body:
            continue
        
        instrs = parse_instructions(body)
        
        # 0x4 blr
        if size == 0x4:
            results.append({
                'tid': tid, 'unit': unit, 'sym': sym, 'src': src,
                'size': '0x4', 'kind': 'blr',
                'ret': 'void', 'stub': f'extern "C" void {sym}(void) {{}}'
            })
            continue
        
        # 0x8 patterns
        if size == 0x8:
            info = classify_0x8(instrs, sym, unit)
            if info is None:
                continue
            
            kind = info[0]
            result = {
                'tid': tid, 'unit': unit, 'sym': sym, 'src': src,
                'size': '0x8', 'kind': kind,
            }
            
            if kind == 'thunk-li':
                result['thunk_imm'] = info[1]
                result['thunk_target'] = info[2]
            elif kind in ('thunk-addi', 'thunk-subi'):
                result['thunk_offset'] = info[1]
                result['thunk_target'] = info[2]
            elif kind == 'thunk-load':
                result['thunk_offset'] = info[1]
                result['thunk_target'] = info[2]
            elif kind == 'int':
                result['ret'] = 'int'
                result['body'] = info[1]
            elif kind in ('u8', 'u16', 'u32', 's16'):
                result['ret'] = kind
                result['body'] = info[1]
            elif kind == 'float':
                result['ret'] = 'float'
                result['body'] = info[1]
            elif kind == 'void*':
                result['ret'] = 'void*'
                result['body'] = info[1]
            elif kind == 'void':
                result['ret'] = 'void'
                result['body'] = info[1]
                result['param_type'] = info[2]
                result['body'] = info[1]  # body already correct
                # Re-build with param
                result['stub'] = f'extern "C" void {sym}(void* self, {info[2]} val) {{ {info[1]} }}'
                result['_has_stub'] = True
            
            if '_has_stub' not in result:
                # Build the stub for non-void functions
                stub = f'extern "C" {result.get("ret", "int")} {sym}(void* self) {{ {result.get("body", "return 0;")} }}'
                result['stub'] = stub
            
            results.append(result)
    
    return results

def claim_targets(data, results):
    """Claim targets in batches."""
    for i, r in enumerate(results):
        tid = r['tid']
        try:
            proc = subprocess.run(
                ['python3', 'tools/coop/run.py', 'targets', 'claim', tid, '--owner', 'pi-agent'],
                capture_output=True, text=True, timeout=8
            )
            if 'claimed' in proc.stdout:
                print(f'  [{i+1}/{len(results)}] ✓ {tid} ({r["sym"]})')
            else:
                print(f'  [{i+1}/{len(results)}] ✗ {tid}: {proc.stdout[:80]}')
        except Exception as e:
            print(f'  [{i+1}/{len(results)}] ! {tid}: {e}')

def edit_sources(results):
    """Edit source files with proper C++ code."""
    # Group by source file
    from collections import OrderedDict
    by_src = OrderedDict()
    for r in results:
        if 'stub' not in r:
            continue
        if r['src'] not in by_src:
            by_src[r['src']] = []
        by_src[r['src']].append(r)
    
    for src, targets in by_src.items():
        with open(src) as f:
            content = f.read()
        
        new_content = content
        edits = 0
        
        for r in targets:
            tid = r['tid']
            stub = r['stub']
            
            begin_marker = f'// LLM-HARNESS-BEGIN: {tid}'
            end_marker = f'// LLM-HARNESS-END: {tid}'
            
            begin_idx = new_content.find(begin_marker)
            if begin_idx == -1:
                print(f'  SKIP marker {tid} in {src}')
                continue
            end_idx = new_content.find(end_marker, begin_idx)
            if end_idx == -1:
                continue
            
            block_start = new_content.rfind('\n', 0, begin_idx) + 1
            block_end = new_content.find('\n', end_idx + len(end_marker)) + 1
            if block_end == 0:
                block_end = len(new_content)
            
            new_block = f'{begin_marker}\n{stub}\n{end_marker}\n'
            new_content = new_content[:block_start] + new_block + new_content[block_end:]
            edits += 1
        
        if edits > 0:
            with open(src, 'w') as f:
                f.write(new_content)
            print(f'  Edited {src}: {edits} targets')
        else:
            print(f'  No changes for {src}')

def cycle_one(tid):
    """Cycle one target with timeout."""
    try:
        proc = subprocess.run(
            ['python3', 'tools/coop/run.py', 'cycle', tid,
             '--hypothesis', 'Auto batch decomp', '--next-change', 'Automated getter/setter/thunk'],
            capture_output=True, text=True, timeout=60
        )
        result = proc.stdout + proc.stderr
        if 'PASS' in result:
            return 'PASS'
        elif 'FAIL' in result or 'fail' in result.lower():
            if 'build' in result.lower() or 'ninja' in result.lower():
                return 'BUILD_FAIL'
            return 'FAIL'
        else:
            return 'INC'
    except (subprocess.TimeoutExpired, subprocess.SubprocessError):
        return 'TIMEOUT'
    except Exception as e:
        return f'ERROR: {e}'

def do_claim():
    print('=== CLAIM PHASE ===')
    data = load_targets()
    results = find_targets(data)
    print(f'Found {len(results)} claimable targets')
    claim_targets(data, results)

def do_edit():
    print('=== EDIT PHASE ===')
    data = load_targets()
    results = find_targets(data)
    # But results needs to include already-claimed targets too
    # Re-scan from scratch
    results = find_targets(data)
    print(f'Found {len(results)} editable targets')
    edit_sources(results)

def do_cycle():
    print('=== CYCLE PHASE ===')
    data = load_targets()
    # Cycle one target per unit that's CLAIMED (pi-agent)
    by_unit = {}
    for t in data['targets']:
        if t.get('workflow_status') == 'CLAIMED' and t.get('claim',{}).get('owner') == 'pi-agent':
            unit = t.get('unit','')
            if unit not in by_unit:
                by_unit[unit] = []
            by_unit[unit].append(t['id'])
    
    for unit, ids in sorted(by_unit.items()):
        tid = ids[0]
        print(f'  Cycling {tid} ({unit})...', end=' ', flush=True)
        result = cycle_one(tid)
        print(result)
        if result == 'BUILD_FAIL':
            print(f'    Skipping remaining {len(ids)-1} targets in {unit}')
            continue
        # Cycle remaining in same unit
        for tid2 in ids[1:]:
            print(f'  Cycling {tid2} ({unit})...', end=' ', flush=True)
            result = cycle_one(tid2)
            print(result)

def do_cycle_claimed():
    """
    Cycle ALL targets that are CLAIMED by pi-agent but not yet cycled.
    Groups by TU and cycles all at once to avoid rebuild.
    """
    print('=== CYCLE ALL CLAIMED ===')
    data = load_targets()
    by_unit = {}
    for t in data['targets']:
        if t.get('workflow_status') == 'CLAIMED' and t.get('claim',{}).get('owner') == 'pi-agent':
            unit = t.get('unit','')
            if unit not in by_unit:
                by_unit[unit] = []
            by_unit[unit].append(t['id'])
    
    total = sum(len(v) for v in by_unit.values())
    done = 0
    for unit, ids in sorted(by_unit.items()):
        for tid in ids:
            print(f'  [{done+1}/{total}] {tid} ({unit})...', end=' ', flush=True)
            result = cycle_one(tid)
            print(result)
            done += 1
            if 'BUILD_FAIL' in result:
                print(f'    Skipping remaining {len(ids)-ids.index(tid)-1} targets in {unit}')
                break

def do_status():
    data = load_targets()
    raw = find_targets(data)
    claimed_mine = sum(1 for t in data['targets'] if t.get('workflow_status') == 'CLAIMED' and t.get('claim',{}).get('owner') == 'pi-agent')
    accepted = sum(1 for t in data['targets'] if t.get('workflow_status') == 'ACCEPTED' and t.get('claim',{}).get('owner') == 'pi-agent')
    full_match = sum(1 for t in data['targets'] if t.get('status') == 'FULL_MATCH')
    not_started = sum(1 for t in data['targets'] if t.get('status') == 'NOT_STARTED')
    
    print(f'NOT_STARTED in registry: {not_started}')
    print(f'FULL_MATCH total:       {full_match}')
    print(f'ACCEPTED (pi-agent):    {accepted}')
    print(f'CLAIMED (pi-agent):     {claimed_mine}')
    print(f'Auto-detectable now:    {len(raw)}')
    
    # Show categories
    from collections import Counter
    kinds = Counter(r['kind'] for r in raw)
    print(f'\nPattern breakdown of claimable targets:')
    for kind, count in sorted(kinds.items(), key=lambda x: -x[1]):
        print(f'  {count:4d}x  {kind}')

def do_all():
    print('=== BATCH DEPLOY ===')
    data = load_targets()
    results = find_targets(data)
    print(f'Found {len(results)} targets')
    claim_targets(data, results)
    edit_sources(results)
    print('\nNow cycling...')
    do_cycle_claimed()

if __name__ == '__main__':
    cmd = sys.argv[1] if len(sys.argv) > 1 else 'status'
    if cmd == 'claim': do_claim()
    elif cmd == 'edit': do_edit()
    elif cmd == 'cycle': do_cycle_claimed()
    elif cmd == 'status': do_status()
    elif cmd == 'all': do_all()
    else: print(f'Unknown command: {cmd}')
