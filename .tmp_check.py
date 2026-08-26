import json, re, subprocess

units = {}
for line in open('.tmp_units.tsv'):
    parts = line.rstrip('\n').split('\t')
    units[parts[0]] = int(parts[3])  # unmatched count

out = subprocess.run(['/Applications/Paseo.app/Contents/Resources/bin/paseo','ls','--json'],
                     capture_output=True, text=True).stdout
agents = [a for a in json.loads(out) if a['name'].startswith('decomp:')]

def find_unit(title):
    if title in units:
        return title
    cand = re.split(r':\s*', title, 1)[-1].strip()
    if cand in units:
        return cand
    matches = [u for u in units if u in title]
    return matches

results = []
for a in agents:
    title = a['name'][len('decomp: '):]
    st = a['status']
    if st == 'running':
        results.append((a['shortId'], st, title, '-', 'LEAVE-RUNNING'))
        continue
    u = find_unit(title)
    if isinstance(u, str):
        unm = units[u]
        action = 'DONE' if unm == 0 else 'NUDGE'
        results.append((a['shortId'], st, title, f'{u} (unmatched={unm})', action))
    elif isinstance(u, list) and len(u) >= 1:
        tot_unm = sum(units[x] for x in u)
        action = 'DONE' if tot_unm == 0 else 'NUDGE'
        results.append((a['shortId'], st, title, '+'.join(u)+f' (unmatched={tot_unm})', action))
    else:
        results.append((a['shortId'], st, title, 'NO UNIT MATCH', 'MANUAL'))

for r in results:
    print('\t'.join(str(x) for x in r))
