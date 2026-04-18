#!/usr/bin/env python3
# /// script
# requires-python = ">=3.12"
# ///
"""テスト結果の組み立て・集約 CLI (collect-run-results.ts の Python 版)

サブコマンド:
  build-entry   ケース記録から1テスト分の結果 JSON を stdout に出力
  carry-over    前回結果から再実行不要分を JSONL にコピー
  finalize      JSONL → JSON 配列に変換
"""
import argparse
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from lib.run_results import build_result_entry, carry_over_results, finalize_results


def cmd_build_entry(args: argparse.Namespace) -> None:
    entry = build_result_entry(
        file=args.file,
        problem=args.problem,
        environment=args.environment,
        status=args.status,
        last_execution_time=args.last_execution_time,
        compile_error=args.compile_error,
        compile_error_file=args.compile_error_file,
        cases_records=args.cases_records,
    )
    sys.stdout.write(json.dumps(entry, ensure_ascii=False) + "\n")


def cmd_carry_over(args: argparse.Namespace) -> None:
    carried = carry_over_results(
        prev_result=args.prev_result,
        need_rerun_file=args.need_rerun_file,
        split_tests_file=args.split_tests_file,
        env=args.env,
        out_jsonl=args.out_jsonl,
    )
    print(f"Carried over {carried} results from previous run")


def cmd_finalize(args: argparse.Namespace) -> None:
    count = finalize_results(in_jsonl=args.in_jsonl, out_json=args.out_json)
    print(f"Results written to {args.out_json} ({count} tests)")


def main() -> None:
    parser = argparse.ArgumentParser()
    sub = parser.add_subparsers(dest="command", required=True)

    p_build = sub.add_parser("build-entry")
    p_build.add_argument("--file", required=True)
    p_build.add_argument("--problem", required=True)
    p_build.add_argument("--environment", required=True)
    p_build.add_argument("--status", required=True)
    p_build.add_argument("--last-execution-time", required=True)
    p_build.add_argument("--compile-error")
    p_build.add_argument("--compile-error-file")
    p_build.add_argument("--cases-records")
    p_build.set_defaults(func=cmd_build_entry)

    p_carry = sub.add_parser("carry-over")
    p_carry.add_argument("--prev-result", required=True)
    p_carry.add_argument("--need-rerun-file", required=True)
    p_carry.add_argument("--split-tests-file", required=True)
    p_carry.add_argument("--env", required=True)
    p_carry.add_argument("--out-jsonl", required=True)
    p_carry.set_defaults(func=cmd_carry_over)

    p_fin = sub.add_parser("finalize")
    p_fin.add_argument("--in-jsonl", required=True)
    p_fin.add_argument("--out-json", required=True)
    p_fin.set_defaults(func=cmd_finalize)

    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
