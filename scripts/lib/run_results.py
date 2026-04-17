"""テスト結果の組み立て・集約モジュール (run-results.ts + results.ts の Python 移植)

build_result_entry: ケース記録ファイルから1テスト分の結果 JSON を構築
finalize_results:   JSONL → JSON 配列へ変換
carry_over_results: 前回結果のうち再実行不要なものを JSONL にコピー

stdlib のみ使用。
"""
import hashlib
import json
import os
from typing import Any

CASE_RECORD_SEPARATOR = "\x1f"


def compute_cases_hash(cases: list[dict]) -> str:
    """ケース名のリストからハッシュを計算する。

    テストケースの追加・削除を検知するために使用。
    同一ハッシュ同士でのみ実行時間の比較が意味を持つ。
    """
    names = sorted(c.get("name", "") for c in cases)
    return hashlib.sha256("\n".join(names).encode()).hexdigest()[:16]


# ============================================================
# results.ts 相当
# ============================================================


def _read_json_file(file_path: str) -> Any:
    if not os.path.exists(file_path):
        return None
    try:
        with open(file_path) as f:
            return json.load(f)
    except (json.JSONDecodeError, OSError):
        return None


def _load_compact_results(raw: Any) -> dict:
    if isinstance(raw, dict) and "tests" in raw and "hpp_map" in raw:
        return raw
    return {"tests": {}, "hpp_map": {}}


def load_compact_results_from_path(file_path: str) -> dict:
    return _load_compact_results(_read_json_file(file_path))


# ============================================================
# run-results.ts 相当
# ============================================================


def _flatten_previous_results(prev_path: str) -> list[dict]:
    compact = load_compact_results_from_path(prev_path)
    flat: list[dict] = []
    for file, test_data in compact["tests"].items():
        for environment, env_data in test_data.get("environments", {}).items():
            cases = env_data.get("cases", [])
            flat.append(
                {
                    "file": file,
                    "problem": test_data.get("problem", ""),
                    "environment": environment,
                    "status": env_data.get("status", ""),
                    "compile_error": env_data.get("compile_error"),
                    "last_execution_time": env_data.get(
                        "last_execution_time", ""
                    ),
                    "cases": cases,
                    "time_max_ms": max((c["time_ms"] for c in cases), default=0),
                    "memory_max_kb": max((c["memory_kb"] for c in cases), default=0),
                    "cases_hash": compute_cases_hash(cases),
                }
            )
    return flat


def _load_lines_as_set(file_path: str) -> set[str]:
    result: set[str] = set()
    if not file_path or not os.path.exists(file_path):
        return result
    with open(file_path) as f:
        for raw_line in f:
            line = raw_line.strip()
            if line and not line.startswith("#"):
                result.add(line)
    return result


def load_case_records(path: str | None) -> list[dict]:
    """ケース記録ファイル (\x1f 区切り) を読み取る"""
    if not path or not os.path.exists(path):
        return []
    cases: list[dict] = []
    with open(path) as f:
        for raw_line in f:
            raw_line = raw_line.rstrip("\n")
            if not raw_line:
                continue
            parts = raw_line.split(CASE_RECORD_SEPARATOR)
            if len(parts) < 4:
                continue
            name, status, time_ms, memory_kb = parts[0], parts[1], parts[2], parts[3]
            detail = parts[4] if len(parts) > 4 else ""
            entry: dict = {
                "name": name,
                "status": status,
                "time_ms": int(time_ms),
                "memory_kb": int(memory_kb),
            }
            if detail:
                entry["detail"] = detail
            cases.append(entry)
    return cases


def build_result_entry(
    *,
    file: str,
    problem: str,
    environment: str,
    status: str,
    last_execution_time: str,
    compile_error: str | None = None,
    compile_error_file: str | None = None,
    cases_records: str | None = None,
) -> dict:
    """1テスト分の結果エントリを構築する"""
    ce = None
    if compile_error_file and os.path.exists(compile_error_file):
        with open(compile_error_file) as f:
            ce = f.read()
    elif compile_error:
        ce = compile_error

    cases = load_case_records(cases_records)
    entry: dict = {
        "file": file,
        "problem": problem,
        "environment": environment,
        "status": status,
        "last_execution_time": last_execution_time,
        "cases": cases,
        "time_max_ms": max((c["time_ms"] for c in cases), default=0),
        "memory_max_kb": max((c["memory_kb"] for c in cases), default=0),
        "cases_hash": compute_cases_hash(cases),
    }
    if ce:
        entry["compile_error"] = ce
    return entry


def carry_over_results(
    *,
    prev_result: str,
    need_rerun_file: str,
    split_tests_file: str,
    env: str,
    out_jsonl: str,
) -> int:
    """前回結果から再実行不要なテストを JSONL にコピーする"""
    if not os.path.exists(prev_result):
        return 0

    need_rerun = _load_lines_as_set(need_rerun_file)
    split_tests = _load_lines_as_set(split_tests_file)
    previous = _flatten_previous_results(prev_result)

    carried = 0
    lines: list[str] = []
    for entry in previous:
        if entry["file"] not in split_tests:
            continue
        if entry["file"] in need_rerun:
            continue
        if entry["environment"] != env:
            continue
        lines.append(json.dumps(entry, ensure_ascii=False) + "\n")
        carried += 1

    if lines:
        with open(out_jsonl, "a") as f:
            f.writelines(lines)

    return carried


def finalize_results(*, in_jsonl: str, out_json: str) -> int:
    """JSONL ファイルを読み込み、JSON 配列として出力する"""
    items: list[dict] = []
    if os.path.exists(in_jsonl):
        with open(in_jsonl) as f:
            for raw_line in f:
                line = raw_line.strip()
                if not line:
                    continue
                items.append(json.loads(line))

    with open(out_json, "w") as f:
        json.dump(items, f, indent=2, ensure_ascii=False)
        f.write("\n")
    return len(items)
