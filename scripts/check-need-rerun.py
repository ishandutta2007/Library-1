#!/usr/bin/env python3
"""
差分実行の判定スクリプト

前回結果 JSON と、テストファイル+依存 hpp の git コミット日時を比較して、
再実行が必要なテストだけを stdout に出力する。
前回結果がないテスト、前回失敗したテスト、依存ファイルが更新されたテストが対象。

使い方:
  python3 scripts/check-need-rerun.py --prev-result PREV.json --test-files FILE... > need_rerun.txt
  python3 scripts/check-need-rerun.py --prev-result PREV.json --all > need_rerun.txt

前回結果がない場合は全テストを出力する。
"""
import argparse
import json
import os
import re
import subprocess
import sys
from datetime import datetime, timezone
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent


def get_git_timestamp(files: list[str]) -> datetime | None:
    """ファイル群の最終コミット日時を取得"""
    if not files:
        return None
    try:
        result = subprocess.run(
            ["git", "log", "-1", "--date=iso-strict", "--pretty=%ad", "--"] + files,
            capture_output=True, text=True, cwd=ROOT
        )
        if result.returncode == 0 and result.stdout.strip():
            return datetime.fromisoformat(result.stdout.strip())
    except Exception:
        pass
    return None


def get_dependencies(test_file: str) -> list[str]:
    """テストファイルの直接依存 hpp を取得"""
    full_path = ROOT / test_file
    if not full_path.exists():
        return []
    content = full_path.read_text()
    return re.findall(r'#include\s+"(src/[^"]+\.hpp)"', content)


def get_hpp_transitive_deps(hpp: str, dep_cache: dict) -> set[str]:
    """hpp の推移的依存を取得"""
    if hpp in dep_cache:
        return dep_cache[hpp]

    dep_cache[hpp] = set()  # 循環回避
    full_path = ROOT / hpp
    if not full_path.exists():
        return set()

    content = full_path.read_text()
    direct_deps = re.findall(r'#include\s+"(src/[^"]+\.hpp)"', content)

    result = set(direct_deps)
    for dep in direct_deps:
        result |= get_hpp_transitive_deps(dep, dep_cache)

    dep_cache[hpp] = result
    return result


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--prev-result", help="前回結果 JSON ファイル")
    parser.add_argument("--test-files", nargs="*", help="判定対象のテストファイル")
    parser.add_argument("--all", action="store_true", help="全テストを対象にする")
    parser.add_argument("--env", default="", help="環境名")
    args = parser.parse_args()

    # 前回結果の読み込み
    prev_results = {}
    if args.prev_result and os.path.exists(args.prev_result):
        with open(args.prev_result) as f:
            data = json.load(f)
        for entry in data:
            prev_results[entry["file"]] = entry

    # テストファイル一覧
    if args.all:
        test_files = []
        test_dir = ROOT / "test"
        for p in sorted(test_dir.rglob("*.test.cpp")):
            test_files.append(str(p.relative_to(ROOT)))
    else:
        test_files = args.test_files or []

    dep_cache = {}
    rerun_count = 0
    skip_count = 0

    for test_file in test_files:
        need_rerun = True

        prev = prev_results.get(test_file)
        if prev and prev.get("status") == "AC":
            # 前回成功 → 変更がなければスキップ
            prev_time_str = prev.get("last_execution_time")
            if prev_time_str:
                prev_time = datetime.fromisoformat(prev_time_str)

                # テストファイル + 全依存の最終コミット日時を取得
                deps = get_dependencies(test_file)
                all_deps = set(deps)
                for hpp in list(deps):
                    all_deps |= get_hpp_transitive_deps(hpp, dep_cache)

                all_files = [test_file] + list(all_deps)
                latest = get_git_timestamp(all_files)

                if latest and latest <= prev_time:
                    need_rerun = False

        if need_rerun:
            print(test_file)
            rerun_count += 1
        else:
            skip_count += 1

    print(f"# Need rerun: {rerun_count}, Skip: {skip_count}", file=sys.stderr)


if __name__ == "__main__":
    main()
