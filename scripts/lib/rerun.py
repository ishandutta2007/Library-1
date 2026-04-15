"""差分実行の判定ロジック (rerun.ts の Python 移植)

前回結果と各テストファイル・依存 hpp の git タイムスタンプを比較し、
再実行が必要なテストを判定する。
"""

import os
import re
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
TEST_DIR = ROOT / "test"


def _read_file_if_exists(file_path: str | Path) -> str:
    p = Path(file_path)
    return p.read_text() if p.exists() else ""


def get_git_timestamp(files: list[str]) -> str | None:
    """ファイル群の最新 git コミット日時を ISO 形式で返す"""
    if not files:
        return None
    result = subprocess.run(
        ["git", "log", "-1", "--date=iso-strict", "--pretty=%ad", "--", *files],
        cwd=ROOT,
        capture_output=True,
        text=True,
    )
    if result.returncode != 0 or not result.stdout.strip():
        return None
    return result.stdout.strip()


def get_dependencies(test_file: str) -> list[str]:
    """テストファイルから直接の #include "mylib/..." 依存を抽出"""
    content = _read_file_if_exists(ROOT / test_file)
    return [m.group(1) for m in re.finditer(r'#include\s+"(mylib/[^"]+\.hpp)"', content)]


def get_hpp_transitive_deps(
    hpp: str, dep_cache: dict[str, set[str]]
) -> set[str]:
    """hpp の推移的依存を再帰的に収集"""
    if hpp in dep_cache:
        return dep_cache[hpp]

    result: set[str] = set()
    dep_cache[hpp] = result
    content = _read_file_if_exists(ROOT / hpp)
    for m in re.finditer(r'#include\s+"(mylib/[^"]+\.hpp)"', content):
        dep = m.group(1)
        result.add(dep)
        result.update(get_hpp_transitive_deps(dep, dep_cache))
    return result


def load_previous_results(
    prev_result_path: str, env: str
) -> dict[str, dict]:
    """前回結果から環境ごとの結果を取得"""
    from lib.run_results import load_compact_results_from_path

    results = load_compact_results_from_path(prev_result_path)
    previous: dict[str, dict] = {}
    for file, test_data in results["tests"].items():
        env_result = test_data.get("environments", {}).get(env)
        if not env_result:
            continue
        previous[file] = {
            "file": file,
            "status": env_result.get("status", ""),
            "last_execution_time": env_result.get("last_execution_time", ""),
        }
    return previous


def list_target_tests(all_tests: bool, explicit_tests: list[str]) -> list[str]:
    """対象テストファイルの一覧を取得"""
    if not all_tests:
        return explicit_tests

    result: list[str] = []
    for test_file in sorted(TEST_DIR.rglob("*.test.cpp")):
        content = test_file.read_text()
        if re.search(r"competitive-verifier:\s*IGNORE", content):
            continue
        result.append(str(test_file.relative_to(ROOT)))
    return result


def compute_need_rerun(
    *,
    prev_result_path: str | None,
    test_files: list[str],
    env: str,
) -> tuple[list[str], int]:
    """再実行が必要なテストを判定。(rerun_list, skipped_count) を返す。"""
    previous = (
        load_previous_results(prev_result_path, env)
        if prev_result_path
        else {}
    )
    dep_cache: dict[str, set[str]] = {}
    rerun: list[str] = []
    skipped = 0

    for test_file in test_files:
        need_rerun = True
        prev = previous.get(test_file)
        if prev and prev["status"] == "AC" and prev["last_execution_time"]:
            prev_time = prev["last_execution_time"]
            direct_deps = get_dependencies(test_file)
            all_deps: set[str] = set(direct_deps)
            for hpp in direct_deps:
                all_deps.update(get_hpp_transitive_deps(hpp, dep_cache))
            latest = get_git_timestamp([test_file, *all_deps])
            if latest and latest <= prev_time:
                need_rerun = False

        if need_rerun:
            rerun.append(test_file)
        else:
            skipped += 1

    return rerun, skipped
