#!/usr/bin/env python3
import re
from pathlib import Path

repo_root = Path(__file__).resolve().parents[1]
report = repo_root / 'cppcheck_report.txt'
if not report.exists():
    print('ERROR: cppcheck_report.txt not found at', report)
    raise SystemExit(1)

text = report.read_text()
pattern = re.compile(r'^(.*?):\d+:\d+: information: Include file: <([^>]+)> not found\.', re.MULTILINE)
entries = pattern.findall(text)
if not entries:
    print('No missing include entries found in cppcheck_report.txt')
    raise SystemExit(0)

# preserve order of appearance
mapping = {}
for path, header in entries:
    p = Path(path)
    key = str((repo_root / p).resolve())
    mapping.setdefault(key, []).append(header)

modified_files = []
for fpath, headers in mapping.items():
    f = Path(fpath)
    if not f.exists():
        print('Warning: file not found, skipping', fpath)
        continue
    src = f.read_text()
    lines = src.splitlines()
    # find insertion index: after last #include if any, else after pragma once or include guard, else at top
    last_inc = -1
    for i, ln in enumerate(lines):
        if ln.strip().startswith('#include'):
            last_inc = i
    insert_idx = last_inc + 1 if last_inc >= 0 else 0
    if insert_idx == 0:
        # look for pragma once or header guard in first 20 lines
        for i, ln in enumerate(lines[:20]):
            s = ln.strip()
            if s.startswith('#pragma once') or (s.startswith('#ifndef') and i+1 < len(lines) and lines[i+1].strip().startswith('#define')):
                insert_idx = i + 1
                break
    # perform insertions in order
    offset = 0
    for h in headers:
        include_line = f'#include <{h}>'
        lines.insert(insert_idx + offset, include_line)
        offset += 1
        print(f'Inserted "{include_line}" into {fpath} at line {insert_idx + offset}')
    new_src = '\n'.join(lines) + ('\n' if not src.endswith('\n') else '\n')
    if new_src != src:
        f.write_text(new_src)
        modified_files.append(fpath)

print('\nSummary:')
print(f'Processed {len(mapping)} files from report, modified {len(modified_files)} files.')
for mf in modified_files:
    print(' M ', mf)
print('\nDone.')
