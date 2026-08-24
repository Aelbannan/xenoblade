import re
fails = open('./monolib_fail.txt').read().strip().splitlines()
monolib_units = []
for line in fails:
    m = re.search(r'unit:\s+([^\s]+)', line)
    if m:
        unit = m.group(1)
        basename = unit.split('/')[-1]
        monolib_units.append((unit, basename, line))
print('FAIL count', len(monolib_units))
for u,b,l in monolib_units:
    print(u, '->', b)
lines = open('./paseo_ls.txt').read().splitlines()[1:]
agents = []
for line in lines:
    if not line.strip():
        continue
    aid = line.split()[0]
    status = None
    for tok in ['running','idle','error','closed']:
        if tok in line:
            status = tok
            break
    agents.append((aid, status, line))
live = [a for a in agents if a[1] != 'closed']
print('total agents', len(agents), 'live', len(live))
for unit, basename, line in monolib_units:
    matches = [a for a in live if basename in a[2]]
    if not matches:
        print(f'DEAD {basename} ({unit}) no live agent')
    else:
        statuses = [a[1] for a in matches]
        ids = [a[0] for a in matches]
        print(f'{basename}: matches {len(matches)} ids={ids} statuses={statuses} -> ', end='')
        idle_matches = [a for a in matches if a[1]=='idle']
        running_matches = [a for a in matches if a[1]=='running']
        if running_matches and not idle_matches:
            print('RUNNING skip')
        elif idle_matches:
            print(f'IDLE woke {idle_matches[0][0]}')
        else:
            print(f'OTHER statuses {statuses}')
print('--- idle live agents ---')
for a in live:
    if a[1]=='idle':
        print(a)
