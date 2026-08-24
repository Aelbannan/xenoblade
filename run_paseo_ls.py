import subprocess, shutil, pathlib
paseo_path = "/System/Volumes/Data/Applications/Paseo.app/Contents/Resources/bin/paseo"
result = subprocess.run([paseo_path, 'ls'], capture_output=True, text=True, timeout=10)
print('STDOUT len', len(result.stdout))
print(result.stdout[:5000])
print('STDERR', result.stderr[:500])
print('return', result.returncode)
open('./fresh_paseo.txt','w').write(result.stdout)
# also try copy diff
try:
    shutil.copy('./fresh_diff.txt', '/tmp/diff_all.txt')
    print('copied diff to /tmp/diff_all.txt')
except Exception as e:
    print('copy failed', e)
    # try via python write
    try:
        data = open('./fresh_diff.txt','r').read()
        open('/tmp/diff_all.txt','w').write(data)
        print('wrote via open')
    except Exception as e2:
        print('write failed', e2)
