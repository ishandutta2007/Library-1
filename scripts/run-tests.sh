#!/usr/bin/env bash
set -euo pipefail

# =============================================================================
# テスト実行スクリプト
# 使い方:
#   ./scripts/run-tests.sh [--env ENV_NAME] [--split-index I] [--split-size N] [--prev-result FILE]
#
# ENV_NAME: "ubuntu-g++", "ubuntu-clang++", "macos-g++", "macos-clang++" など
# 環境変数:
#   CXX        コンパイラ (default: g++)
#   CXXFLAGS   コンパイルフラグ (default: -std=c++17 -O2)
#   TC_DIR     テストケースキャッシュディレクトリ (default: .cache/testcases)
#   RESULT_DIR 結果出力ディレクトリ (default: .cache/results)
# =============================================================================

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
TEST_DIR="${ROOT}/test"
SRC_DIR="${ROOT}/src"

CXX="${CXX:-g++}"
CXXFLAGS="${CXXFLAGS:--std=c++17 -O2}"
TC_DIR="${TC_DIR:-${ROOT}/.cache/testcases}"
RESULT_DIR="${RESULT_DIR:-${ROOT}/.cache/results}"
ENV_NAME="${ENV_NAME:-local}"
SPLIT_INDEX=""
SPLIT_SIZE=""
PREV_RESULT=""

# 引数パース
while [[ $# -gt 0 ]]; do
  case "$1" in
    --env) ENV_NAME="$2"; shift 2 ;;
    --split-index) SPLIT_INDEX="$2"; shift 2 ;;
    --split-size) SPLIT_SIZE="$2"; shift 2 ;;
    --prev-result) PREV_RESULT="$2"; shift 2 ;;
    *) echo "Unknown option: $1"; exit 1 ;;
  esac
done

mkdir -p "${TC_DIR}" "${RESULT_DIR}"

# =============================================================================
# テストケースのダウンロード
# =============================================================================
get_testcase_dir() {
  local problem_url="$1"

  # URL の MD5 ハッシュでキャッシュディレクトリを検索
  local url_md5
  url_md5=$(echo -n "${problem_url}" | md5sum 2>/dev/null | cut -c1-32 || echo -n "${problem_url}" | md5 -q 2>/dev/null)

  # setup ジョブ (download-testcases.py) でダウンロード済みのテストケースを参照
  local cache_dir="${TC_DIR}/${url_md5}"
  if [[ -d "${cache_dir}" ]] && [[ "$(ls -A "${cache_dir}" 2>/dev/null)" ]]; then
    echo "${cache_dir}"
    return 0
  fi

  return 1
}

# =============================================================================
# 1つのテストケースを実行して時間・メモリを計測
# =============================================================================
run_single_case() {
  local binary="$1"
  local input_file="$2"
  local expected_file="$3"
  local tle_sec="$4"
  local error_tolerance="$5"
  local output_file
  output_file=$(mktemp)

  local status="AC"
  local elapsed_ms=0
  local memory_kb=0

  local time_output
  time_output=$(mktemp)

  # 実行 + 計測
  if [[ "$(uname)" == "Darwin" ]]; then
    # macOS: /usr/bin/time -l (メモリはバイト単位)
    /usr/bin/time -l timeout "${tle_sec}" "${binary}" < "${input_file}" > "${output_file}" 2>"${time_output}" && true
    local exit_code=$?

    if [[ ${exit_code} -eq 124 ]] || [[ ${exit_code} -eq 137 ]]; then
      status="TLE"
    elif [[ ${exit_code} -ne 0 ]]; then
      status="RE"
    fi

    # "0.01 real 0.00 user 0.00 sys" の形式
    elapsed_ms=$(awk '/real/{printf "%.0f", $1 * 1000}' "${time_output}" 2>/dev/null || echo "0")
    # メモリはバイト単位 → KB
    memory_kb=$(awk '/maximum resident set size/{printf "%.0f", $1 / 1024}' "${time_output}" 2>/dev/null || echo "0")
  else
    # Linux: /usr/bin/time -v
    /usr/bin/time -v timeout "${tle_sec}" "${binary}" < "${input_file}" > "${output_file}" 2>"${time_output}" && true
    local exit_code=$?

    if [[ ${exit_code} -eq 124 ]] || [[ ${exit_code} -eq 137 ]]; then
      status="TLE"
    elif [[ ${exit_code} -ne 0 ]]; then
      status="RE"
    fi

    elapsed_ms=$(grep "Elapsed (wall clock)" "${time_output}" | sed 's/.*: //' | awk -F: '{if (NF==2) printf "%.0f", ($1*60+$2)*1000; else printf "%.0f", $1*1000}' 2>/dev/null || echo "0")
    # Linux の time -v は KB 単位
    memory_kb=$(grep "Maximum resident set size" "${time_output}" | awk '{print $NF}' 2>/dev/null || echo "0")
  fi

  rm -f "${time_output}"

  # 空値のガード
  [[ -z "${elapsed_ms}" ]] && elapsed_ms=0
  [[ -z "${memory_kb}" ]] && memory_kb=0

  # 判定 (TLE/RE でなければ出力を比較)
  if [[ "${status}" == "AC" ]]; then
    if [[ -n "${error_tolerance}" ]] && [[ "${error_tolerance}" != "0" ]]; then
      # 浮動小数点比較
      if ! python3 -c "
import sys
with open('${output_file}') as f: actual = f.read().split()
with open('${expected_file}') as f: expected = f.read().split()
if len(actual) != len(expected): sys.exit(1)
for a, e in zip(actual, expected):
    if abs(float(a) - float(e)) > ${error_tolerance}: sys.exit(1)
" 2>/dev/null; then
        status="WA"
      fi
    else
      # 完全一致比較 (末尾空白・改行は無視)
      if ! diff <(sed 's/[[:space:]]*$//' "${output_file}") <(sed 's/[[:space:]]*$//' "${expected_file}") &>/dev/null; then
        status="WA"
      fi
    fi
  fi

  rm -f "${output_file}"

  echo "${status} ${elapsed_ms} ${memory_kb}"
}

# =============================================================================
# テストファイルのアノテーションを解析
# =============================================================================
parse_annotations() {
  local test_file="$1"
  PROBLEM_URL=""
  TLE_SEC="10"
  MLE_MB="256"
  ERROR_TOL=""
  IS_IGNORE=false
  IS_STANDALONE=false

  while IFS= read -r line; do
    if [[ "${line}" =~ competitive-verifier:\ *PROBLEM\ +(.+) ]]; then
      PROBLEM_URL="${BASH_REMATCH[1]}"
    elif [[ "${line}" =~ competitive-verifier:\ *TLE\ +([0-9.]+) ]]; then
      TLE_SEC="${BASH_REMATCH[1]}"
    elif [[ "${line}" =~ competitive-verifier:\ *MLE\ +([0-9]+) ]]; then
      MLE_MB="${BASH_REMATCH[1]}"
    elif [[ "${line}" =~ competitive-verifier:\ *ERROR\ +([0-9.]+) ]]; then
      ERROR_TOL="${BASH_REMATCH[1]}"
    elif [[ "${line}" =~ competitive-verifier:\ *IGNORE ]]; then
      IS_IGNORE=true
    elif [[ "${line}" =~ competitive-verifier:\ *STANDALONE ]]; then
      IS_STANDALONE=true
    fi
  done < <(head -20 "${test_file}")
}

# =============================================================================
# 1つのテストファイルを実行
# =============================================================================
run_test_file() {
  local test_file="$1"
  local rel_path
  rel_path="${test_file#${ROOT}/}"

  parse_annotations "${test_file}"

  # IGNORE はスキップ
  if [[ "${IS_IGNORE}" == true ]]; then
    echo "  [SKIP] ${rel_path} (IGNORE)"
    return
  fi

  # コンパイル
  local binary
  binary=$(mktemp)
  if ! ${CXX} ${CXXFLAGS} -I"${ROOT}" -o "${binary}" "${test_file}" 2>/dev/null; then
    echo "  [CE] ${rel_path}"
    # CE の結果を記録
    cat <<JSONEOF >> "${RESULT_FILE}"
,{
  "file": "${rel_path}",
  "problem": "${PROBLEM_URL}",
  "environment": "${ENV_NAME}",
  "status": "CE",
  "last_execution_time": "${EXECUTION_TIME}",
  "cases": []
}
JSONEOF
    rm -f "${binary}"
    return
  fi

  if [[ "${IS_STANDALONE}" == true ]]; then
    # STANDALONE: テストケース不要、プログラム自体がテストを実行
    echo -n "  [RUN] ${rel_path} (standalone) ... "
    local result
    result=$(run_single_case "${binary}" /dev/null /dev/null "${TLE_SEC}" "")
    local case_status
    case_status=$(echo "${result}" | awk '{print $1}')
    local case_time
    case_time=$(echo "${result}" | awk '{print $2}')
    local case_mem
    case_mem=$(echo "${result}" | awk '{print $3}')

    # standalone は exit code 0 なら AC
    if [[ "${case_status}" == "AC" ]]; then
      echo "AC (${case_time}ms, ${case_mem}KB)"
    else
      echo "${case_status} (${case_time}ms, ${case_mem}KB)"
    fi

    cat <<JSONEOF >> "${RESULT_FILE}"
,{
  "file": "${rel_path}",
  "problem": "${PROBLEM_URL}",
  "environment": "${ENV_NAME}",
  "status": "${case_status}",
  "last_execution_time": "${EXECUTION_TIME}",
  "cases": [{"name": "standalone", "status": "${case_status}", "time_ms": ${case_time}, "memory_kb": ${case_mem}}]
}
JSONEOF
    rm -f "${binary}"
    return
  fi

  # テストケース取得
  if [[ -z "${PROBLEM_URL}" ]]; then
    echo "  [SKIP] ${rel_path} (no PROBLEM URL)"
    rm -f "${binary}"
    return
  fi

  local tc_cache
  tc_cache=$(get_testcase_dir "${PROBLEM_URL}")
  if [[ -z "${tc_cache}" ]]; then
    echo "  [SKIP] ${rel_path} (testcases unavailable)"
    rm -f "${binary}"
    return
  fi

  # テストケース実行
  echo -n "  [RUN] ${rel_path} "
  local overall_status="AC"
  local cases_json=""
  local case_count=0

  shopt -s nullglob
  for input_file in "${tc_cache}"/*/in.txt "${tc_cache}"/*.in; do
    [[ -f "${input_file}" ]] || continue

    # 期待出力ファイルを探す
    local expected_file=""
    if [[ "${input_file}" == */in.txt ]]; then
      expected_file="$(dirname "${input_file}")/out.txt"
    elif [[ "${input_file}" == *.in ]]; then
      expected_file="${input_file%.in}.out"
    fi
    [[ -f "${expected_file}" ]] || continue

    # ケース名を抽出
    local case_name
    if [[ "${input_file}" == */in.txt ]]; then
      case_name=$(basename "$(dirname "${input_file}")")
    else
      case_name=$(basename "${input_file}" .in)
    fi

    local result
    result=$(run_single_case "${binary}" "${input_file}" "${expected_file}" "${TLE_SEC}" "${ERROR_TOL}")
    local case_status
    case_status=$(echo "${result}" | awk '{print $1}')
    local case_time
    case_time=$(echo "${result}" | awk '{print $2}')
    local case_mem
    case_mem=$(echo "${result}" | awk '{print $3}')

    if [[ "${case_status}" != "AC" ]]; then
      overall_status="${case_status}"
    fi

    [[ -n "${cases_json}" ]] && cases_json+=","
    cases_json+="{\"name\":\"${case_name}\",\"status\":\"${case_status}\",\"time_ms\":${case_time},\"memory_kb\":${case_mem}}"
    case_count=$((case_count + 1))
  done

  echo "${overall_status} (${case_count} cases)"

  cat <<JSONEOF >> "${RESULT_FILE}"
,{
  "file": "${rel_path}",
  "problem": "${PROBLEM_URL}",
  "environment": "${ENV_NAME}",
  "status": "${overall_status}",
  "last_execution_time": "${EXECUTION_TIME}",
  "cases": [${cases_json}]
}
JSONEOF

  rm -f "${binary}"
}

# =============================================================================
# メイン
# =============================================================================

# テストファイル一覧を作成
ALL_TESTS=()
while IFS= read -r line; do
  ALL_TESTS+=("$line")
done < <(find "${TEST_DIR}" -name '*.test.cpp' | sort)
TOTAL=${#ALL_TESTS[@]}

# 分割実行
if [[ -n "${SPLIT_INDEX}" ]] && [[ -n "${SPLIT_SIZE}" ]]; then
  CHUNK_SIZE=$(( (TOTAL + SPLIT_SIZE - 1) / SPLIT_SIZE ))
  START=$(( SPLIT_INDEX * CHUNK_SIZE ))
  END=$(( START + CHUNK_SIZE ))
  [[ ${END} -gt ${TOTAL} ]] && END=${TOTAL}
  TESTS=("${ALL_TESTS[@]:${START}:$((END - START))}")
  echo "Split ${SPLIT_INDEX}/${SPLIT_SIZE}: tests ${START}-$((END-1)) of ${TOTAL}"
else
  TESTS=("${ALL_TESTS[@]}")
  echo "Running all ${TOTAL} tests"
fi

# 差分実行: 前回結果がある場合、変更がないテストはスキップ
NEED_RERUN_FILE=$(mktemp)
if [[ -n "${PREV_RESULT}" ]] && [[ -f "${PREV_RESULT}" ]]; then
  echo "Checking for changes against previous result..."
  # テストファイル一覧を一時ファイルに書き出し
  TESTS_LIST=$(mktemp)
  for t in "${TESTS[@]}"; do
    echo "${t#${ROOT}/}" >> "${TESTS_LIST}"
  done
  python3 "${ROOT}/scripts/check-need-rerun.py" \
    --prev-result "${PREV_RESULT}" \
    --env "${ENV_NAME}" \
    --test-files $(cat "${TESTS_LIST}") \
    > "${NEED_RERUN_FILE}" 2>&1
  rm -f "${TESTS_LIST}"
  echo "$(grep -c -v '^#' "${NEED_RERUN_FILE}" || echo 0) tests need re-running"
else
  # 前回結果なし → 全テスト実行
  for t in "${TESTS[@]}"; do
    echo "${t#${ROOT}/}" >> "${NEED_RERUN_FILE}"
  done
fi

RESULT_FILE="${RESULT_DIR}/result-${ENV_NAME}.json"
echo "[null" > "${RESULT_FILE}"

# 前回結果からスキップしたテストの結果をコピー
if [[ -n "${PREV_RESULT}" ]] && [[ -f "${PREV_RESULT}" ]]; then
  python3 -c "
import json, sys
need_rerun = set()
with open('${NEED_RERUN_FILE}') as f:
    for line in f:
        line = line.strip()
        if line and not line.startswith('#'):
            need_rerun.add(line)
with open('${PREV_RESULT}') as f:
    prev = json.load(f)
carried = 0
with open('${RESULT_FILE}', 'a') as out:
    for entry in prev:
        if entry['file'] not in need_rerun:
            out.write(',' + json.dumps(entry) + '\n')
            carried += 1
print(f'Carried over {carried} results from previous run', file=sys.stderr)
" 2>&1
fi

echo "Environment: ${ENV_NAME} (${CXX})"
echo "---"

EXECUTION_TIME=$(date -u +"%Y-%m-%dT%H:%M:%S+00:00")

for test_file in "${TESTS[@]}"; do
  local_rel="${test_file#${ROOT}/}"
  # 差分実行: need_rerun に含まれていなければスキップ
  if ! grep -q "^${local_rel}$" "${NEED_RERUN_FILE}" 2>/dev/null; then
    echo "  [CACHED] ${local_rel}"
    continue
  fi
  run_test_file "${test_file}"
done

rm -f "${NEED_RERUN_FILE}"

# JSON の先頭の null を削除して配列を閉じる
echo "]" >> "${RESULT_FILE}"
# null, を削除して整形
python3 -c "
import json, sys
with open('${RESULT_FILE}') as f:
    data = json.load(f)
data = [x for x in data if x is not None]
with open('${RESULT_FILE}', 'w') as f:
    json.dump(data, f, indent=2)
print(f'Results written to ${RESULT_FILE} ({len(data)} tests)')
"
