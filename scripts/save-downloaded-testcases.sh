#!/usr/bin/env bash
set -euo pipefail

# ダウンロードしたテストケースをサービスごとに zip にして Releases にアップロード
# yosupo はリポジトリから再生成できるので含めない

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
TC_CACHE="${ROOT}/.cache/testcases"
RELEASE_TAG="testcases"

if [ ! -d "${TC_CACHE}" ]; then
  echo "No testcases to save"
  exit 0
fi

cd "${TC_CACHE}"

# 各テストケースディレクトリがどのサービス由来か判定するための情報を収集
# download-testcases.py が URL の MD5 でディレクトリを作っているので、
# テストファイルからURL→MD5→サービスのマッピングを作る
python3 -c "
import hashlib, re, json
from pathlib import Path

root = Path('${ROOT}')
mapping = {}  # md5 → service

for test_file in sorted((root / 'test').rglob('*.test.cpp')):
    content = test_file.read_text()
    m = re.search(r'competitive-verifier:\s*PROBLEM\s+(\S+)', content)
    if not m:
        continue
    url = m.group(1)
    md5 = hashlib.md5(url.encode()).hexdigest()

    if 'onlinejudge.u-aizu.ac.jp' in url:
        mapping[md5] = 'aoj'
    elif 'yukicoder.me' in url:
        mapping[md5] = 'yukicoder'
    elif 'judge.yosupo.jp' in url:
        mapping[md5] = 'yosupo'
    elif 'hackerrank.com' in url:
        mapping[md5] = 'hackerrank'
    else:
        mapping[md5] = 'other'

with open('${TC_CACHE}/.service-mapping.json', 'w') as f:
    json.dump(mapping, f)
"

# サービスごとにディレクトリを振り分けて zip
for service in aoj yukicoder hackerrank other; do
  dirs=()
  for d in */; do
    d="${d%/}"
    # yosupo (testlib.h がある) はスキップ
    [ -f "${d}/testlib.h" ] && continue
    # サービスマッピングから判定
    s=$(python3 -c "
import json
with open('.service-mapping.json') as f:
    m = json.load(f)
print(m.get('${d}', 'unknown'))
" 2>/dev/null || echo "unknown")
    if [ "${s}" = "${service}" ]; then
      dirs+=("${d}")
    fi
  done

  if [ ${#dirs[@]} -eq 0 ]; then
    continue
  fi

  zip_file="${ROOT}/.cache/tc-${service}.zip"
  rm -f "${zip_file}"
  zip -q -r "${zip_file}" "${dirs[@]}"
  zip_size=$(du -h "${zip_file}" | cut -f1)
  echo "Created tc-${service}.zip (${zip_size}, ${#dirs[@]} problems)"

  if command -v gh &>/dev/null; then
    gh release view "${RELEASE_TAG}" &>/dev/null || \
      gh release create "${RELEASE_TAG}" --title "Testcase Cache" --notes "Auto-generated testcase cache for CI"
    gh release delete-asset "${RELEASE_TAG}" "tc-${service}.zip" --yes 2>/dev/null || true
    gh release upload "${RELEASE_TAG}" "${zip_file}" --clobber
    echo "Uploaded tc-${service}.zip to release ${RELEASE_TAG}"
  fi
done

rm -f "${TC_CACHE}/.service-mapping.json"
echo "Done"
