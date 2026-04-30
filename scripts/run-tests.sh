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
SCRIPTS_DIR="${ROOT}/scripts"

# 共通関数を読み込み
source "${SCRIPTS_DIR}/lib/run-lib.sh"

CXX="${CXX:-g++}"
CXXFLAGS="${CXXFLAGS:--std=c++17 -O2}"
read -r -a CXX_CMD_ARR <<< "${CXX}"
read -r -a CXXFLAGS_ARR <<< "${CXXFLAGS}"

# macOS clang + libc++ には __lg が無いので互換ヘッダを強制 include する。
# Linux gcc/libstdc++ では clang_compat.hpp 内の polyfill は無効化される。
if [[ "$(uname)" == "Darwin" ]]; then
  CXXFLAGS_ARR+=("-include" "${ROOT}/include/clang_compat.hpp")
fi
TC_DIR="${TC_DIR:-${ROOT}/.cache/testcases}"
RESULT_DIR="${RESULT_DIR:-${ROOT}/.cache/results}"
ENV_NAME="${ENV_NAME:-local}"
SPLIT_INDEX=""
SPLIT_SIZE=""
SPLIT_FILE=""
PREV_RESULT="${PREV_RESULT:-}"

# 引数パース
while [[ $# -gt 0 ]]; do
  case "$1" in
    --env) ENV_NAME="$2"; shift 2 ;;
    --split-index) SPLIT_INDEX="$2"; shift 2 ;;
    --split-size) SPLIT_SIZE="$2"; shift 2 ;;
    --split-file) SPLIT_FILE="$2"; shift 2 ;;
    --prev-result) PREV_RESULT="$2"; shift 2 ;;
    *) echo "Unknown option: $1"; exit 1 ;;
  esac
done

mkdir -p "${TC_DIR}" "${RESULT_DIR}"

# =============================================================================
# Python ヘルパー呼び出し
# =============================================================================
run_python() {
  python3 "$@"
}

append_result_jsonl() {
  local json="$1"
  printf '%s\n' "${json}" >> "${RESULT_JSONL_FILE}"
}

build_result_entry() {
  local file="$1"
  local problem="$2"
  local status="$3"
  local cases_records="$4"
  local compile_error_file="${5:-}"

  local cmd=(python3 "${SCRIPTS_DIR}/collect-run-results.py" build-entry
    --file "${file}"
    --problem "${problem}"
    --environment "${ENV_NAME}"
    --status "${status}"
    --last-execution-time "${EXECUTION_TIME}"
    --cases-records "${cases_records}")

  if [[ -n "${compile_error_file}" ]]; then
    cmd+=(--compile-error-file "${compile_error_file}")
  fi

  "${cmd[@]}"
}

# =============================================================================
# テストケースのダウンロード
# =============================================================================
get_testcase_dir() {
  local problem_url="$1"

  # URL の MD5 ハッシュでキャッシュディレクトリを検索
  local url_md5
  url_md5=$(echo -n "${problem_url}" | md5sum 2>/dev/null | cut -c1-32 || echo -n "${problem_url}" | md5 -q 2>/dev/null)

  # API ダウンロード分 (.cache/testcases/)
  local cache_dir="${TC_DIR}/${url_md5}"
  if [[ -d "${cache_dir}" ]] && [[ "$(ls -A "${cache_dir}" 2>/dev/null)" ]]; then
    echo "${cache_dir}"
    return 0
  fi

  # tc.zip 展開分 (.cache/tc-resolved/)
  local resolved_dir="${ROOT}/.cache/tc-resolved/${url_md5}"
  if [[ -d "${resolved_dir}" ]] && [[ "$(ls -A "${resolved_dir}" 2>/dev/null)" ]]; then
    echo "${resolved_dir}"
    return 0
  fi

  return 1
}

# =============================================================================
# テストファイルのアノテーションを解析
# =============================================================================
parse_annotations() {
  local test_file="$1"
  PROBLEM_URL=""
  TLE_SEC="10"
  export MLE_MB="256"
  ERROR_TOL=""
  IS_IGNORE=false
  IS_STANDALONE=false

  while IFS= read -r line; do
    if [[ "${line}" =~ competitive-verifier:\ *PROBLEM\ +(.+) ]]; then
      PROBLEM_URL="${BASH_REMATCH[1]}"
    elif [[ "${line}" =~ competitive-verifier:\ *TLE\ +([0-9.]+) ]]; then
      TLE_SEC="${BASH_REMATCH[1]}"
    elif [[ "${line}" =~ competitive-verifier:\ *MLE\ +([0-9]+) ]]; then
      export MLE_MB="${BASH_REMATCH[1]}"
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
  rel_path="${test_file#"$ROOT"/}"

  parse_annotations "${test_file}"

  # IGNORE はスキップ
  if [[ "${IS_IGNORE}" == true ]]; then
    echo "  [SKIP] ${rel_path} (IGNORE)"
    return
  fi

  # コンパイル
  local binary
  binary=$(mktemp)
  local compile_err
  compile_err=$(mktemp)
  if ! "${CXX_CMD_ARR[@]}" "${CXXFLAGS_ARR[@]}" -I"${ROOT}" -o "${binary}" "${test_file}" 2>"${compile_err}"; then
    echo "  [CE] ${rel_path}"
    head -20 "${compile_err}" | sed 's/^/    /'
    local compile_error_excerpt
    compile_error_excerpt=$(mktemp)
    head -50 "${compile_err}" > "${compile_error_excerpt}"
    append_result_jsonl "$(build_result_entry "${rel_path}" "${PROBLEM_URL}" "CE" /dev/null "${compile_error_excerpt}")"
    rm -f "${compile_error_excerpt}"
    rm -f "${binary}" "${compile_err}"
    return
  fi
  rm -f "${compile_err}"

  if [[ "${IS_STANDALONE}" == true ]]; then
    # STANDALONE: テストケース不要、プログラム自体がテストを実行
    echo -n "  [RUN] ${rel_path} (standalone) ... "
    local result
    result=$(run_single_case "${binary}" /dev/null /dev/null "${TLE_SEC}" "")
    local case_status case_time case_mem case_detail
    read -r case_status case_time case_mem case_detail <<< "${result}"

    if [[ "${case_status}" == "AC" ]]; then
      echo "AC (${case_time}ms, ${case_mem}KB)"
    else
      echo "${case_status} (${case_time}ms, ${case_mem}KB)"
    fi

    local case_records_file
    case_records_file=$(mktemp)
    append_case_record "${case_records_file}" "standalone" "${case_status}" "${case_time}" "${case_mem}" "${case_detail:-}"
    append_result_jsonl "$(build_result_entry "${rel_path}" "${PROBLEM_URL}" "${case_status}" "${case_records_file}")"
    rm -f "${case_records_file}"
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
  tc_cache=$(get_testcase_dir "${PROBLEM_URL}" || true)
  if [[ -z "${tc_cache}" ]]; then
    echo "  [SKIP] ${rel_path} (testcases unavailable)"
    rm -f "${binary}"
    return
  fi

  # テストケース実行
  echo -n "  [RUN] ${rel_path} "
  local overall_status="AC"
  local case_count=0
  local case_records_file
  case_records_file=$(mktemp)

  # checker の検出・コンパイル (yosupo judge 等)
  local checker_bin
  checker_bin=$(compile_checker "${tc_cache}")

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

    # DETAIL_LOG_DIR をエクスポートしておく
    export DETAIL_LOG_DIR="${ROOT}/.cache/logs/${ENV_NAME}/${rel_path}"
    mkdir -p "${DETAIL_LOG_DIR}" 2>/dev/null || true

    local result
    result=$(case_name="${case_name}" run_single_case "${binary}" "${input_file}" "${expected_file}" "${TLE_SEC}" "${ERROR_TOL}" "${checker_bin}")
    local case_status case_time case_mem case_detail
    read -r case_status case_time case_mem case_detail <<< "${result}"

    if [[ "${case_status}" != "AC" ]]; then
      overall_status="${case_status}"
    fi

    append_case_record "${case_records_file}" "${case_name}" "${case_status}" "${case_time}" "${case_mem}" "${case_detail:-}"
    case_count=$((case_count + 1))
  done

  echo "${overall_status} (${case_count} cases)"

  append_result_jsonl "$(build_result_entry "${rel_path}" "${PROBLEM_URL}" "${overall_status}" "${case_records_file}")"

  rm -f "${case_records_file}"
  rm -f "${binary}"
}

# =============================================================================
# メイン
# =============================================================================

# テストファイル一覧を作成
TESTS=()

if [[ -n "${SPLIT_FILE}" ]] && [[ -f "${SPLIT_FILE}" ]]; then
  # split ファイルから読み込み (各行が test/... の相対パス)
  while IFS= read -r line; do
    [[ -n "${line}" ]] && TESTS+=("${ROOT}/${line}")
  done < "${SPLIT_FILE}"
  echo "Running ${#TESTS[@]} tests from ${SPLIT_FILE}"
elif [[ -n "${SPLIT_INDEX}" ]] && [[ -n "${SPLIT_SIZE}" ]]; then
  # インデックスベースの分割
  ALL_TESTS=()
  while IFS= read -r line; do
    ALL_TESTS+=("$line")
  done < <(find "${TEST_DIR}" -name '*.test.cpp' | sort)
  TOTAL=${#ALL_TESTS[@]}
  CHUNK_SIZE=$(( (TOTAL + SPLIT_SIZE - 1) / SPLIT_SIZE ))
  START=$(( SPLIT_INDEX * CHUNK_SIZE ))
  END=$(( START + CHUNK_SIZE ))
  [[ ${END} -gt ${TOTAL} ]] && END=${TOTAL}
  TESTS=("${ALL_TESTS[@]:${START}:$((END - START))}")
  echo "Split ${SPLIT_INDEX}/${SPLIT_SIZE}: tests ${START}-$((END-1)) of ${TOTAL}"
else
  while IFS= read -r line; do
    TESTS+=("$line")
  done < <(find "${TEST_DIR}" -name '*.test.cpp' | sort)
  echo "Running all ${#TESTS[@]} tests"
fi

# 差分実行: 前回結果がある場合、変更がないテストはスキップ
NEED_RERUN_FILE=$(mktemp)
if [[ -n "${PREV_RESULT}" ]] && [[ -f "${PREV_RESULT}" ]]; then
  echo "Checking for changes against previous result..."
  # テストファイル一覧を一時ファイルに書き出し
  SPLIT_TESTS_FILE=$(mktemp)
  for t in "${TESTS[@]}"; do
    echo "${t#"$ROOT"/}" >> "${SPLIT_TESTS_FILE}"
  done
  mapfile -t split_test_args < "${SPLIT_TESTS_FILE}"
  run_python "${SCRIPTS_DIR}/check-need-rerun.py" \
    --prev-result "${PREV_RESULT}" \
    --env "${ENV_NAME}" \
    --test-files "${split_test_args[@]}" \
    > "${NEED_RERUN_FILE}" 2>&1
  echo "$(grep -c -v '^#' "${NEED_RERUN_FILE}" || echo 0) tests need re-running"
else
  # 前回結果なし → 全テスト実行
  for t in "${TESTS[@]}"; do
    echo "${t#"$ROOT"/}" >> "${NEED_RERUN_FILE}"
  done
fi

RESULT_JSONL_FILE="${RESULT_DIR}/result-${ENV_NAME}.jsonl"
RESULT_FILE="${RESULT_DIR}/result-${ENV_NAME}.json"
: > "${RESULT_JSONL_FILE}"

# 前回結果からスキップしたテストの結果をコピー
if [[ -n "${PREV_RESULT}" ]] && [[ -f "${PREV_RESULT}" ]]; then
  run_python "${SCRIPTS_DIR}/collect-run-results.py" carry-over \
    --prev-result "${PREV_RESULT}" \
    --need-rerun-file "${NEED_RERUN_FILE}" \
    --split-tests-file "${SPLIT_TESTS_FILE}" \
    --env "${ENV_NAME}" \
    --out-jsonl "${RESULT_JSONL_FILE}" \
    || true
fi

echo "Environment: ${ENV_NAME} (${CXX})"
echo "---"

EXECUTION_TIME=$(date -u +"%Y-%m-%dT%H:%M:%S+00:00")

for test_file in "${TESTS[@]}"; do
  local_rel="${test_file#"$ROOT"/}"
  # 差分実行: need_rerun に含まれていなければスキップ
  if ! grep -q "^${local_rel}$" "${NEED_RERUN_FILE}" 2>/dev/null; then
    echo "  [CACHED] ${local_rel}"
    continue
  fi
  run_test_file "${test_file}"
done

rm -f "${NEED_RERUN_FILE}" "${SPLIT_TESTS_FILE:-}"

run_python "${SCRIPTS_DIR}/collect-run-results.py" finalize \
  --in-jsonl "${RESULT_JSONL_FILE}" \
  --out-json "${RESULT_FILE}"
