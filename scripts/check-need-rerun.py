#!/usr/bin/env python3
"""差分実行判定 CLI (check-need-rerun.ts の Python 版)

再実行が必要なテストファイルを1行1ファイルで stdout に出力する。
"""
import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from lib.rerun import compute_need_rerun, list_target_tests


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--prev-result")
    parser.add_argument("--env", default="")
    parser.add_argument("--all", action="store_true")
    parser.add_argument("--test-files", nargs="*", default=[])
    args = parser.parse_args()

    test_files = list_target_tests(args.all, args.test_files)
    rerun, skipped = compute_need_rerun(
        prev_result_path=args.prev_result,
        test_files=test_files,
        env=args.env,
    )

    for f in rerun:
        print(f)
    print(f"# Need rerun: {len(rerun)}, Skip: {skipped}", file=sys.stderr)


if __name__ == "__main__":
    main()
