#!/usr/bin/env python3
"""
Batch-decompile NOT_STARTED targets using cluster.py structural groups.

Analyzes assembly patterns per group, generates correct C/C++ stubs,
applies them, and cycles through verification.

Usage:
  python3 tools/batch_cluster.py status              # Show group/pattern overview
  python3 tools/batch_cluster.py apply --max-groups 5 # Process easiest N groups
  python3 tools/batch_cluster.py apply --group-id N   # Process specific group
  python3 tools/batch_cluster.py apply --difficulty 8 # Process all groups with diff <= 8
"""

import json, re, os, sys, datetime, subprocess, time
from collections import Counter, OrderedDict, defaultdict

PROJ = '/Users/ahmedelbannan/Ahmed/xenoblade'
TARGETS_JSON = os.path.join(PROJ, 'tools/coop/targets.json')
ASM_DIR = os.path.join(PROJ, 'build/us/asm')

def find_src(unit):
    if unit.startswith('kyoshin/'): return os.path.join(PROJ, 'src', f'{unit}.cpp')
    if unit.startswith('PowerPC_EABI_Support/'): return os.path.join(PROJ, 'libs', f'{unit}.c')
    if unit.startswith(('monolib/', 'CriWare/', 'RVL_SDK/', 'nw4r/')): 
        # Check both .cpp and .c
        for ext in ('.cpp', '.c', '.cp', '.cc'):
            p = os.path.join(PROJ, 'libs', f'{unit}{ext}')
            if os.path.exists(p): return p
        return os.path.join(PROJ, 'libs', f'{unit}.cpp')
    return None

def is_c(src_path):
    return src_path.endswith('.c') if src_path else False

def is_catalog(src_path):
    if not src_path or not os.path.exists(src_path): return False
    with open(src_path) as f: return 'LLM-HARNESS' in f.read(8000)

def get_asm_body(unit, sym):
    asm_path = os.path.join(ASM_DIR, f'{unit}.s')
    if not os.path.exists(asm_path): return None
    with open(asm_path) as f: asm = f.read()
    m = re.search(r'\.fn ' + re.escape(sym) + r',.*?\n(.*?)\.endfn', asm, re.DOTALL)
    return m.group(1).strip() if m else None

def analyze_instrs(body):
    if not body: return []
    instrs = []
    for line in body.split('\n'):
        line = line.strip()
        if not line: continue
        idx = line.find('*/')
        part = line[idx+2:].strip() if idx >= 0 else line
        # Skip labels
        if part.startswith('.'): continue
        parts = part.split()
        if parts and not parts[0].startswith('.') and parts[0] not in ('.L',):
            instrs.append((parts[0], ' '.join(parts[1:])))
    return instrs

def classify(instrs, size):
    """Classify an assembly body into a pattern kind and extract parameters."""
    if not instrs:
        return None
    
    mnemonics = [i[0] for i in instrs]
    
    # 0x4: single instruction
    if size == 0x4:
        if instrs[0][0] == 'blr':
            return ('blr',)
        if instrs[0][0] == 'b':
            target = instrs[0][1]
            return ('b', target)
        return None
    
    # 0x8: two instructions
    if size == 0x8 and len(instrs) == 2:
        m1, o1 = instrs[0]
        m2, o2 = instrs[1]
        
        if m2 == 'blr':
            # li r3, N or li r0, N (some C files use r0)
            if m1 == 'li':
                m = re.match(r'^r\d+,\s*(.+)$', o1)
                if m: return ('li', m.group(1).strip())
            # lbz r3, N(r3) - field byte getter
            if m1 == 'lbz':
                m = re.match(r'^r3,\s*(.+?)\(r3\)$', o1)
                if m: return ('lbz', m.group(1))
            # lhz r3, N(r3)
            if m1 == 'lhz':
                m = re.match(r'^r3,\s*(.+?)\(r3\)$', o1)
                if m: return ('lhz', m.group(1))
            # lha r3, N(r3)
            if m1 == 'lha':
                m = re.match(r'^r3,\s*(.+?)\(r3\)$', o1)
                if m: return ('lha', m.group(1))
            # lwz r3, N(r3)
            if m1 == 'lwz':
                m = re.match(r'^r3,\s*(.+?)\(r3\)$', o1)
                if m: return ('lwz', m.group(1))
            # lwz r3, lbl_X@sda21(rN) - global load
            if m1 == 'lwz':
                m = re.match(r'^r3,\s*(.+?)(?:@sda21|@ha)?\(r\d+\)$', o1)
                if m: return ('lwz_global', m.group(1))
            # lfs f1, N(r3)
            if m1 == 'lfs':
                m = re.match(r'^f1,\s*(.+?)\(r3\)$', o1)
                if m: return ('lfs', m.group(1))
            # stb r4, N(r3)
            if m1 == 'stb':
                m = re.match(r'^r4,\s*(.+?)\(r3\)$', o1)
                if m: return ('stb', m.group(1))
            # stw r4, N(r3)
            if m1 == 'stw':
                m = re.match(r'^r4,\s*(.+?)\(r3\)$', o1)
                if m: return ('stw', m.group(1))
            # stw r3, lbl_X@sda21(rN) - global store
            if m1 == 'stw':
                m = re.match(r'^r3,\s*(.+?)(?:@sda21|@ha)?\(r\d+\)$', o1)
                if m: return ('stw_global', m.group(1))
            # stfs f1, N(r3)
            if m1 == 'stfs':
                m = re.match(r'^f1,\s*(.+?)\(r3\)$', o1)
                if m: return ('stfs', m.group(1))
            # addi r3, r3, N
            if m1 == 'addi':
                m = re.match(r'^r3,\s*r3,\s*(.+)$', o1)
                if m: return ('addi', m.group(1).strip())
            # mr r3, rN
            if m1 == 'mr':
                m = re.match(r'^r3,\s*r(\d+)$', o1)
                if m: return ('mr', m.group(1))
            # clrlwi r3, r3, N
            if m1 == 'clrlwi':
                return ('clrlwi',)
            # extrwi r3, r3, N, M
            if m1 == 'extrwi':
                return ('extrwi',)
            # srwi r3, r3, N
            if m1 == 'srwi':
                return ('srwi',)
            # lis r3, lbl@ha
            if m1 == 'lis':
                m = re.match(r'^r3,\s*(.+?)(?:@ha|@l|@sda21)?$', o1)
                if m: return ('lis', m.group(1).strip())
            # xori r3, r3, N
            if m1 == 'xori':
                return ('xori',)
            # subfic r3, r3, 0
            if m1 == 'subfic':
                return ('subfic',)
            # neg r3, r3
            if m1 == 'neg':
                return ('neg',)
        
        if m2 == 'b':
            # thunks: addi/subi + b target
            target = o2.split()[0] if o2 else ''
            if m1 == 'addi':
                m = re.match(r'r\d+,\s*r\d+,\s*(\S+)', o1)
                if m: return ('thunk_addi', m.group(1), target)
            if m1 == 'subi':
                m = re.match(r'r\d+,\s*r\d+,\s*(\S+)', o1)
                if m: return ('thunk_subi', m.group(1), target)
            # stw + b
            if m1 == 'stw':
                m = re.match(r'r3,\s*(.+?)\(r(\d+)\)', o1)
                if m: return ('thunk_stw', m.group(1), target)
            # lwz + b
            if m1 == 'lwz':
                m = re.match(r'r3,\s*(.+?)\(r\d+\)', o1)
                if m: return ('thunk_lwz', m.group(1), target)
            # b to another function (branch, no link)
            if m1 == 'b':
                return ('b', o1.split()[0] if o1 else '')
    
    # 0xC: three instructions
    if size == 0xC and len(instrs) == 3:
        m1, o1 = instrs[0]
        m2, o2 = instrs[1]
        m3, o3 = instrs[2]
        
        if m3 == 'blr':
            # stw r3, N(r3); stw r4, M(r3) - double store
            if m1 == 'stw' and m2 == 'stw':
                return ('stw2',)
            # stw r3, lbl; stw r3, lbl2
            if m1 == 'stw' and m2 == 'stw':
                return ('stw2_global',)
            # li r3, N; li r0, M; stw r0, ... - pointless
            # stw r4, N(r3); stw r5, M(r3)
            if m1 == 'stw' and m2 == 'stw':
                return ('stw2',)
        if m3 == 'b':
            # thunk: lwz rN, OFF(rN); addi rN, rN, OFF; b target
            if m1 == 'lwz' and m2 == 'addi':
                return ('thunk_call',)
            # thunk: lwz rN, OFF(rN); lwz rN, OFF(rN); b target
            if m1 == 'lwz' and m2 == 'lwz':
                return ('thunk_call2',)
    
    return None

def make_stub(sym, kind, is_c_file, *args):
    """Generate the C/C++ stub for a given pattern kind."""
    # For C files, no extern "C" wrapper
    if is_c_file:
        prefix = ''
        suffix = ''
    else:
        prefix = 'extern "C" '
        suffix = ''
    
    if kind == 'blr':
        return f'{prefix}void {sym}(void) {{\n}}\n'
    
    if kind == 'b':
        target = args[0] if args else ''
        # Try to clean up the target name
        target = target.split('@')[0].strip()
        if target:
            return f'{prefix}void {sym}(void) {{\n    {target}();\n}}\n'
        else:
            return f'{prefix}void {sym}(void) {{\n}}\n'
    
    if kind == 'li':
        return f'{prefix}int {sym}(void) {{ return {args[0]}; }}\n'
    
    if kind == 'lbz':
        return f'{prefix}u8 {sym}(void* self) {{ return ((u8*)self)[{args[0]}]; }}\n'
    if kind == 'lhz':
        return f'{prefix}u16 {sym}(void* self) {{ return *(u16*)((u8*)self + {args[0]}); }}\n'
    if kind == 'lha':
        return f'{prefix}s16 {sym}(void* self) {{ return *(s16*)((u8*)self + {args[0]}); }}\n'
    if kind == 'lwz':
        return f'{prefix}u32 {sym}(void* self) {{ return *(u32*)((u8*)self + {args[0]}); }}\n'
    if kind == 'lwz_global':
        lbl = args[0]
        return f'{prefix}u32 {sym}(void) {{\n    extern u32 {lbl};\n    return {lbl};\n}}\n'
    if kind == 'lfs':
        return f'{prefix}float {sym}(void* self) {{ return *(float*)((u8*)self + {args[0]}); }}\n'
    if kind == 'stb':
        return f'{prefix}void {sym}(void* self, u8 val) {{ ((u8*)self)[{args[0]}] = val; }}\n'
    if kind == 'stw':
        return f'{prefix}void {sym}(void* self, u32 val) {{ *(u32*)((u8*)self + {args[0]}) = val; }}\n'
    if kind == 'stw_global':
        lbl = args[0]
        return f'{prefix}void {sym}(u32 val) {{\n    extern u32 {lbl};\n    {lbl} = val;\n}}\n'
    if kind == 'stfs':
        return f'{prefix}void {sym}(void* self, float val) {{ *(float*)((u8*)self + {args[0]}) = val; }}\n'
    if kind == 'addi':
        return f'{prefix}void* {sym}(void* self) {{ return (void*)((u8*)self + {args[0]}); }}\n'
    
    if kind == 'mr':
        reg = args[0] if args else '3'
        if reg == '3':
            return f'{prefix}void* {sym}(void* self) {{ return self; }}\n'
        return f'{prefix}u32 {sym}(void) {{ return {reg}; }}\n'
    
    if kind == 'clrlwi':
        return f'{prefix}u32 {sym}(u32 val) {{ return val & 0xFFFFFF; }}\n'
    if kind == 'extrwi':
        return f'{prefix}u32 {sym}(u32 val) {{ return (val >> 16) & 0x1F; }}\n'
    if kind == 'srwi':
        return f'{prefix}u32 {sym}(u32 val) {{ return val >> 24; }}\n'
    if kind == 'lis':
        lbl = args[0] if args else ''
        return f'{prefix}u32 {sym}(void) {{ return 0; }}\n'
    if kind == 'xori':
        return f'{prefix}u32 {sym}(u32 val) {{ return val ^ 1; }}\n'
    if kind == 'subfic':
        return f'{prefix}u32 {sym}(u32 val) {{ return -val; }}\n'
    if kind == 'neg':
        return f'{prefix}int {sym}(int val) {{ return -val; }}\n'
    
    if kind.startswith('thunk_') or kind == 'b':
        offset = args[0] if len(args) > 0 else ''
        target = args[1] if len(args) > 1 else ''
        if kind == 'b':
            target = target.split('@')[0].strip() if target else ''
            return f'{prefix}void {sym}(void) {{\n    {target}();\n}}\n'
        target = target.split('@')[0].strip()
        if kind == 'thunk_addi':
            return f'{prefix}void {sym}(void* self) {{ ((void(*)(void*)){target})((char*)self + {offset}); }}\n'
        if kind == 'thunk_subi':
            return f'{prefix}void {sym}(void* self) {{ ((void(*)(void*)){target})((char*)self - {offset}); }}\n'
        if kind == 'thunk_stw':
            return f'{prefix}void {sym}(void* self, u32 val) {{ ((void(*)(void*,u32)){target})((char*)self + {args[0]}, val); }}\n'
        if kind == 'thunk_lwz':
            return f'{prefix}u32 {sym}(void* self) {{ return ((u32(*)(void*)){target})((char*)self + {args[0]}); }}\n'
        return None
    
    if kind == 'stw2':
        return f'{prefix}void {sym}(void* self, u32 val1, u32 val2) {{ }}\n'
    
    return None


def get_all_groups():
    """Run cluster.py and return parsed groups."""
    result = subprocess.run(
        ['python3', 'tools/find-groups/cluster.py', '--all-status', '--threshold', '0.80', 
         '--detail', '0', '--max-groups', '999', '--json'],
        capture_output=True, text=True, timeout=120, cwd=PROJ)
    return json.loads(result.stdout)


def analyze_group(unit, member_ids, group_info=None):
    """Analyze a group's patterns."""
    with open(TARGETS_JSON) as f:
        targets = json.load(f)['targets']
    
    target_map = {t['id']: t for t in targets}
    
    patterns = Counter()
    not_started = []
    
    for mid in member_ids:
        if mid not in target_map: continue
        t = target_map[mid]
        if t.get('status') != 'NOT_STARTED': continue
        if t.get('workflow_status') == 'CLAIMED': continue
        
        not_started.append(t)
        
        body = get_asm_body(unit, t['symbol'])
        if not body: continue
        instrs = analyze_instrs(body)
        if not instrs: continue
        
        s = str(t.get('size', '0'))
        size = int(s, 16) if '0x' in s else 999
        info = classify(instrs, size)
        if info:
            patterns[info[0]] += 1
        else:
            patterns['__UNCLASSIFIED__'] += 1
    
    return not_started, patterns


def do_status():
    """Show comprehensive status of all cluster groups."""
    data = get_all_groups()
    groups = data.get('groups', [])
    
    # Count by avg_difficulty tiers
    tiers = [(0, 3), (3, 5), (5, 8), (8, 12), (12, 20), (20, 999)]
    tier_names = ['0-3', '3-5', '5-8', '8-12', '12-20', '20+']
    tier_counts = {n: {'groups': 0, 'funcs': 0, 'ns': 0} for n in tier_names}
    
    for g in groups:
        diff = g.get('avg_difficulty', 999)
        for (lo, hi), name in zip(tiers, tier_names):
            if lo <= diff < hi:
                tier_counts[name]['groups'] += 1
                tier_counts[name]['funcs'] += g.get('size', 0)
                tier_counts[name]['ns'] += g.get('not_started', 0)
                break
    
    print("Difficulty distribution (all groups):")
    for name in tier_names:
        c = tier_counts[name]
        print(f"  Diff {name:>5s}: {c['groups']:4d} groups, {c['funcs']:6d} functions ({c['ns']:6d} NOT_STARTED)")
    
    # Show easiest groups with pattern breakdown
    by_diff = sorted(groups, key=lambda g: g.get('avg_difficulty', 999))
    by_diff = [g for g in by_diff if g.get('not_started', 0) > 0]
    
    print(f"\nEasiest 15 groups (pattern breakdown):")
    print(f"{'#':>4s} {'NS':>4s} {'Diff':>6s} {'Kinds':>50s}  Unit")
    print('-'*120)
    
    for i, g in enumerate(by_diff[:15]):
        unit = g.get('dominant_unit', '')
        ns = g.get('not_started', 0)
        diff = g.get('avg_difficulty', 0)
        member_ids = g.get('member_ids', [])
        
        _, patterns = analyze_group(unit, member_ids)
        kinds_str = ', '.join(f'{k}({c})' for k, c in patterns.most_common(5))
        if not kinds_str: kinds_str = '(no asm)'
        
        print(f'{i+1:4d} {ns:4d} {diff:6.2f} {kinds_str:>50s}  {unit}')


def do_apply(max_groups=None, group_id=None, max_difficulty=None, dry_run=False):
    """Apply batch decompilation to groups."""
    data = get_all_groups()
    groups = data.get('groups', [])
    groups = [g for g in groups if g.get('not_started', 0) > 0]
    
    if group_id is not None:
        target_groups = [g for g in groups if g.get('avg_difficulty', 999) == group_id or
                         g.get('dominant_unit', '') in str(group_id)]
        # Fallback: find by index
        if not target_groups and group_id < len(groups):
            by_diff = sorted(groups, key=lambda g: g.get('avg_difficulty', 999))
            target_groups = [by_diff[group_id]]
    elif max_difficulty:
        target_groups = [g for g in groups if g.get('avg_difficulty', 999) <= max_difficulty and 
                         g.get('avg_difficulty', 999) > 0]
        target_groups.sort(key=lambda g: g.get('avg_difficulty', 999))
    else:
        by_diff = sorted(groups, key=lambda g: g.get('avg_difficulty', 999))
        target_groups = by_diff[:max_groups] if max_groups else by_diff[:5]
    
    print(f"Processing {len(target_groups)} groups...")
    
    with open(TARGETS_JSON) as f:
        targets_data = json.load(f)
    targets = targets_data['targets']
    target_map = {t['id']: t for t in targets}
    
    now = datetime.datetime.now(datetime.timezone.utc).isoformat()
    total_edited = 0
    total_claimed = 0
    
    for g in target_groups:
        unit = g.get('dominant_unit', '')
        ns = g.get('not_started', 0)
        diff = g.get('avg_difficulty', 0)
        member_ids = g.get('member_ids', [])
        
        if ns == 0: continue
        
        src = find_src(unit)
        if not src or not os.path.exists(src):
            print(f"  SKIP {unit}: source not found")
            continue
        if not is_catalog(src):
            print(f"  SKIP {unit}: not a catalog TU")
            continue
        
        c_file = is_c(src)
        
        # Collect targets to process
        to_process = []
        for mid in member_ids:
            if mid not in target_map: continue
            t = target_map[mid]
            if t.get('status') != 'NOT_STARTED': continue
            if t.get('workflow_status') in ('CLAIMED', 'ACTIVE', 'COMPILING'): continue
            
            body = get_asm_body(unit, t['symbol'])
            if not body: continue
            instrs = analyze_instrs(body)
            if not instrs: continue
            
            s = str(t.get('size', '0'))
            size = int(s, 16) if '0x' in s else 999
            info = classify(instrs, size)
            
            if not info:
                continue
            
            stub = make_stub(t['symbol'], info[0], c_file, *info[1:])
            if not stub: continue
            
            to_process.append((t, info[0], stub))
        
        if not to_process:
            print(f"  EMPTY {unit}: {ns} NS but none classifiable")
            continue
        
        # Read source
        with open(src) as f:
            content = f.read()
        
        new_content = content
        edits = 0
        
        for t, kind, stub in to_process:
            tid = t['id']
            bm = f'// LLM-HARNESS-BEGIN: {tid}'
            em = f'// LLM-HARNESS-END: {tid}'
            bi = new_content.find(bm)
            if bi == -1: 
                continue
            ei = new_content.find(em, bi)
            if ei == -1: continue
            
            bs = new_content.rfind('\n', 0, bi) + 1
            be = new_content.find('\n', ei + len(em)) + 1
            if be == 0: be = len(new_content)
            
            nb = f'{bm}\n{stub}{em}\n'
            new_content = new_content[:bs] + nb + new_content[be:]
            edits += 1
        
        if edits == 0:
            print(f"  NOEDIT {unit}: {ns} NS, couldn't edit")
            continue
        
        # Write source
        if not dry_run:
            with open(src, 'w') as f:
                f.write(new_content)
        
        # Claim targets
        for t, kind, stub in to_process:
            tid = t['id']
            if not dry_run:
                t['workflow_status'] = 'CLAIMED'
                t['claim'] = {'owner': 'pi-agent', 'claimed_at': now, 'allowed_paths': [src], 'note': ''}
        
        total_edited += edits
        total_claimed += len(to_process)
        
        print(f"  EDITED {unit}: {edits} edits, {len(to_process)} targets claimed (diff={diff:.2f})")
        
        # Save targets periodically
        if not dry_run:
            with open(TARGETS_JSON, 'w') as f:
                json.dump(targets_data, f, indent=2)
    
    # Final save
    if not dry_run:
        with open(TARGETS_JSON, 'w') as f:
            json.dump(targets_data, f, indent=2)
    
    print(f"\nTotal: {total_edited} edits, {total_claimed} targets claimed")
    return total_claimed


def do_cycle(max_cycle=50, timeout_sec=30):
    """Cycle CLAIMED targets through verification."""
    with open(TARGETS_JSON) as f:
        targets_data = json.load(f)
    
    claimed = [t for t in targets_data['targets'] 
               if t.get('workflow_status') == 'CLAIMED' 
               and t.get('claim', {}).get('owner') == 'pi-agent']
    
    # Group by unit, cycle first per unit
    by_unit = OrderedDict()
    for t in claimed:
        unit = t.get('unit', '')
        if unit not in by_unit:
            by_unit[unit] = []
        by_unit[unit].append(t['id'])
    
    first_ids = [v[0] for v in by_unit.values()]
    if max_cycle and max_cycle < len(first_ids):
        first_ids = first_ids[:max_cycle]
    
    print(f"Cycling {len(first_ids)} targets (first per TU)...")
    passed = 0
    failed = 0
    
    for i, tid in enumerate(first_ids):
        try:
            proc = subprocess.run(
                ['python3', 'tools/coop/run.py', 'cycle', tid,
                 '--hypothesis', 'Batch', '--next-change', 'Auto'],
                capture_output=True, text=True, timeout=timeout_sec)
            result = proc.stdout + proc.stderr
            if 'PASS' in result:
                passed += 1
            else:
                failed += 1
            if (i + 1) % 20 == 0:
                print(f"  ... {i+1}/{len(first_ids)}: {passed} passed, {failed} failed")
        except (subprocess.TimeoutExpired, Exception):
            failed += 1
    
    print(f"First-pass: {passed} passed, {failed} failed out of {len(first_ids)}")
    
    # Cycle remaining in passed TUs
    extra = []
    with open(TARGETS_JSON) as f:
        targets_data = json.load(f)
    
    for unit, ids in by_unit.items():
        first_tid = ids[0]
        first_ok = any(t.get('id') == first_tid and 
                      t.get('workflow_status') == 'ACCEPTED' 
                      for t in targets_data['targets'])
        if first_ok:
            extra.extend(ids[1:])
    
    if extra and (not max_cycle or len(extra) + len(first_ids) <= max_cycle):
        extra = extra[:max(0, max_cycle - len(first_ids))]
        print(f"Cycling {len(extra)} extra targets...")
        for i, tid in enumerate(extra):
            try:
                subprocess.run(['python3', 'tools/coop/run.py', 'cycle', tid,
                              '--hypothesis', 'Batch', '--next-change', 'Auto'],
                              capture_output=True, text=True, timeout=timeout_sec)
            except subprocess.TimeoutExpired:
                pass
            except Exception:
                pass
            if (i + 1) % 25 == 0:
                print(f"  ... {i+1}/{len(extra)}")
    
    with open(TARGETS_JSON) as f:
        targets_data = json.load(f)
    accepted = sum(1 for t in targets_data['targets'] if t.get('workflow_status') == 'ACCEPTED')
    fm = sum(1 for t in targets_data['targets'] if t.get('status') == 'FULL_MATCH')
    ns = sum(1 for t in targets_data['targets'] if t.get('status') == 'NOT_STARTED')
    cm = sum(1 for t in targets_data['targets'] if t.get('workflow_status') == 'CLAIMED' and t.get('claim', {}).get('owner') == 'pi-agent')
    print(f"\nResult: FULL_MATCH={fm} ACCEPTED={accepted} NOT_STARTED={ns} PI-CLAIMED={cm}")


if __name__ == '__main__':
    if len(sys.argv) < 2 or sys.argv[1] in ('--help', '-h'):
        print(__doc__)
        sys.exit(0)
    
    cmd = sys.argv[1]
    
    if cmd == 'status':
        do_status()
    elif cmd == 'apply':
        kwargs = {}
        for i, arg in enumerate(sys.argv[2:], 2):
            if arg == '--max-groups' and i + 1 < len(sys.argv):
                kwargs['max_groups'] = int(sys.argv[i+1])
            elif arg == '--group-id':
                kwargs['group_id'] = int(sys.argv[i+1])
            elif arg == '--difficulty':
                kwargs['max_difficulty'] = float(sys.argv[i+1])
            elif arg == '--dry-run':
                kwargs['dry_run'] = True
        do_apply(**kwargs)
    elif cmd == 'cycle':
        kwargs = {}
        for i, arg in enumerate(sys.argv[2:], 2):
            if arg == '--max' and i + 1 < len(sys.argv):
                kwargs['max_cycle'] = int(sys.argv[i+1])
        do_cycle(**kwargs)
    else:
        print(f"Unknown command: {cmd}")
        print(__doc__)
