#!/usr/bin/env bash
# =============================================================================
# テスト実行共通関数 (Library / judge 共有)
#
# 使い方: source scripts/lib/run-lib.sh  (または source lib/scripts/lib/run-lib.sh)
#
# 呼び出し元が設定すべき変数:
#   SCRIPTS_DIR     scripts/ ディレクトリのパス (compare-float-output.py の位置特定用)
#
# 任意設定:
#   MLE_MB          メモリ制限 (MB)。未設定なら MLE 判定しない
#   DETAIL_LOG_DIR  エラーログ出力先。未設定ならログ出力しない
# =============================================================================

# SCRIPTS_DIR が未設定なら、このファイルの親の親を使う
SCRIPTS_DIR="${SCRIPTS_DIR:-$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)}"

# =============================================================================
# ケース記録の追記 (\x1f 区切り)
# =============================================================================
append_case_record() {
  local records_file="$1"
  local case_name="$2"
  local case_status="$3"
  local case_time="$4"
  local case_mem="$5"
  local case_detail="${6:-}"
  printf '%s\x1f%s\x1f%s\x1f%s\x1f%s\n' \
    "${case_name}" "${case_status}" "${case_time}" "${case_mem}" "${case_detail}" \
    >> "${records_file}"
}

# =============================================================================
# checker のコンパイル
# =============================================================================
compile_checker() {
  local tc_dir="$1"
  local checker_bin="${tc_dir}/checker"

  if [[ ! -f "${tc_dir}/checker.cpp" ]]; then
    echo ""
    return
  fi

  if [[ -x "${checker_bin}" ]]; then
    echo "${checker_bin}"
    return
  fi

  local checker_args=(-std=c++17 -O2)
  [[ -f "${tc_dir}/testlib.h" ]] && checker_args+=("-I${tc_dir}")
  if g++ "${checker_args[@]}" -o "${checker_bin}" "${tc_dir}/checker.cpp" 2>/dev/null; then
    echo "${checker_bin}"
  else
    echo ""
  fi
}

# =============================================================================
# 1つのテストケースを実行して時間・メモリを計測
#
# 引数:
#   $1: binary       実行バイナリ
#   $2: input_file   入力ファイル
#   $3: expected_file 期待出力ファイル
#   $4: tle_sec      タイムアウト秒数
#   $5: error_tolerance 浮動小数点誤差許容値 (空文字なら厳密比較)
#   $6: checker_bin  checker バイナリ (空文字なら不使用)
#
# 環境変数 (任意):
#   case_name       ケース名 (ログファイル名に使用)
#   MLE_MB          メモリ制限 (MB)
#   DETAIL_LOG_DIR  エラーログ出力先
#
# stdout: "STATUS TIME_MS MEMORY_KB [DETAIL]"
# =============================================================================
run_single_case() {
  local binary="$1"
  local input_file="$2"
  local expected_file="$3"
  local tle_sec="$4"
  local error_tolerance="$5"
  local checker_bin="${6:-}"
  local output_file
  output_file=$(mktemp)

  local status="AC"
  local elapsed_ms=0
  local memory_kb=0
  local detail=""

  local time_output
  time_output=$(mktemp)
  local stderr_file
  stderr_file=$(mktemp)

  # 実行 + 計測
  # バイナリの stderr を stderr_file に分離し、/usr/bin/time の出力は time_output へ
  if [[ "$(uname)" == "Darwin" ]]; then
    /usr/bin/time -l sh -c 'timeout "$1" "$2" < "$3" > "$4" 2>"$5"' _ \
      "${tle_sec}" "${binary}" "${input_file}" "${output_file}" "${stderr_file}" \
      2>"${time_output}" && true
    local exit_code=$?

    if [[ ${exit_code} -eq 124 ]] || [[ ${exit_code} -eq 137 ]]; then
      status="TLE"
    elif [[ ${exit_code} -ne 0 ]]; then
      status="RE"
      detail="exit code ${exit_code}"
    fi

    elapsed_ms=$(awk '/real/{printf "%.0f", $1 * 1000}' "${time_output}" 2>/dev/null || echo "0")
    memory_kb=$(awk '/maximum resident set size/{printf "%.0f", $1 / 1024}' "${time_output}" 2>/dev/null || echo "0")
  else
    /usr/bin/time -v sh -c 'timeout "$1" "$2" < "$3" > "$4" 2>"$5"' _ \
      "${tle_sec}" "${binary}" "${input_file}" "${output_file}" "${stderr_file}" \
      2>"${time_output}" && true
    local exit_code=$?

    if [[ ${exit_code} -eq 124 ]] || [[ ${exit_code} -eq 137 ]]; then
      status="TLE"
    elif [[ ${exit_code} -ne 0 ]]; then
      status="RE"
      detail="exit code ${exit_code}"
    fi

    elapsed_ms=$(grep "Elapsed (wall clock)" "${time_output}" | sed 's/.*: //' | awk -F: '{if (NF==2) printf "%.0f", ($1*60+$2)*1000; else printf "%.0f", $1*1000}' 2>/dev/null || echo "0")
    memory_kb=$(grep "Maximum resident set size" "${time_output}" | awk '{print $NF}' 2>/dev/null || echo "0")
  fi

  rm -f "${time_output}"

  # 空値のガード
  [[ -z "${elapsed_ms}" ]] && elapsed_ms=0
  [[ -z "${memory_kb}" ]] && memory_kb=0

  # MLE 判定
  if [[ "${status}" == "AC" ]] && [[ -n "${MLE_MB:-}" ]]; then
    local mle_kb=$(( MLE_MB * 1024 ))
    if [[ ${memory_kb} -gt ${mle_kb} ]]; then
      status="MLE"
      detail="used ${memory_kb}KB > limit ${mle_kb}KB"
    fi
  fi

  # 判定 (TLE/RE/MLE でなければ出力を比較)
  if [[ "${status}" == "AC" ]]; then
    if [[ -n "${checker_bin}" ]] && [[ -x "${checker_bin}" ]]; then
      if ! "${checker_bin}" "${input_file}" "${output_file}" "${expected_file}" &>/dev/null; then
        status="WA"
        local actual_head expected_head
        actual_head=$(head -1 "${output_file}" | cut -c1-50)
        expected_head=$(head -1 "${expected_file}" | cut -c1-50)
        detail="expected:[${expected_head}] actual:[${actual_head}]"
      fi
    elif [[ -n "${error_tolerance}" ]] && [[ "${error_tolerance}" != "0" ]]; then
      if ! python3 "${SCRIPTS_DIR}/compare-float-output.py" \
        --actual "${output_file}" \
        --expected "${expected_file}" \
        --tolerance "${error_tolerance}" >/dev/null 2>&1; then
        status="WA"
        detail="float compare failed (tolerance=${error_tolerance})"
      fi
    else
      if ! diff <(sed 's/[[:space:]]*$//' "${output_file}") <(sed 's/[[:space:]]*$//' "${expected_file}") &>/dev/null; then
        status="WA"
        local actual_head expected_head
        actual_head=$(head -1 "${output_file}" | cut -c1-50)
        expected_head=$(head -1 "${expected_file}" | cut -c1-50)
        detail="expected:[${expected_head}] actual:[${actual_head}]"
      fi
    fi
  fi

  # エラー詳細をログディレクトリに保存
  if [[ "${status}" != "AC" ]] && [[ -n "${DETAIL_LOG_DIR:-}" ]]; then
    local log_file="${DETAIL_LOG_DIR}/${case_name:-unknown}.${status}.log"
    {
      echo "Status: ${status}"
      echo "Time: ${elapsed_ms}ms"
      echo "Memory: ${memory_kb}KB"
      if [[ -f "${input_file}" ]]; then
        echo "--- input (first 20 lines) ---"
        head -20 "${input_file}"
      fi
      if [[ "${status}" == "WA" ]] && [[ -f "${output_file}" ]]; then
        echo "--- actual output (first 20 lines) ---"
        head -20 "${output_file}"
        echo "--- expected output (first 20 lines) ---"
        head -20 "${expected_file}"
      fi
      if [[ -s "${stderr_file}" ]]; then
        echo "--- stderr (first 30 lines) ---"
        head -30 "${stderr_file}"
      fi
      echo "--- detail ---"
      echo "${detail}"
    } > "${log_file}" 2>/dev/null
  fi

  rm -f "${output_file}" "${stderr_file}"

  # detail 内の改行や特殊文字をエスケープ
  if [[ -n "${detail}" ]]; then
    detail=$(echo "${detail}" | head -3 | tr '\n' ' ' | sed 's/"/\\"/g' | cut -c1-200)
    echo "${status} ${elapsed_ms} ${memory_kb} ${detail}"
  else
    echo "${status} ${elapsed_ms} ${memory_kb}"
  fi
}
