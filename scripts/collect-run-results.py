#!/usr/bin/env python3
"""
run-tests.sh が出力した中間結果を扱うユーティリティ。

- carry-over: 前回結果から今回未再実行の結果を JSONL に書き出す
- finalize: JSONL を JSON 配列に変換する
"""

from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
from typing import Any


def flatten_previous_results(prev: Any) -> list[dict[str, Any]]:
    flat: list[dict[str, Any]] = []

    if isinstance(prev, dict) and "tests" in prev and "hpp_map" in prev:
        for file_key, test_data in prev["tests"].items():
            envs = test_data.get("environments", {})
            for env_name, env_data in envs.items():
                flat.append(
                    {
                        "file": file_key,
                        "problem": test_data.get("problem", ""),
                        "environment": env_name,
                        "split": test_data.get("split"),
                        "status": env_data.get("status", ""),
                        "compile_error": env_data.get("compile_error"),
                        "last_execution_time": env_data.get("last_execution_time", ""),
                        "cases": env_data.get("cases", []),
                    }
                )
        return flat

    if isinstance(prev, dict):
        for problems in prev.values():
            for problem in problems:
                envs = problem.get("environments", {})
                for env_name, env_data in envs.items():
                    flat.append(
                        {
                            "file": problem.get("file", ""),
                            "problem": problem.get("problem", ""),
                            "environment": env_name,
                            "split": problem.get("split"),
                            "status": env_data.get("status", ""),
                            "compile_error": env_data.get("compile_error"),
                            "last_execution_time": env_data.get("last_execution_time", ""),
                            "cases": env_data.get("cases", []),
                        }
                    )
        return flat

    if isinstance(prev, list):
        return [entry for entry in prev if isinstance(entry, dict)]

    return []


def load_lines_as_set(path: str) -> set[str]:
    result: set[str] = set()
    if not path or not os.path.exists(path):
        return result
    with open(path) as fh:
        for line in fh:
            line = line.strip()
            if line and not line.startswith("#"):
                result.add(line)
    return result


def cmd_carry_over(args: argparse.Namespace) -> int:
    need_rerun = load_lines_as_set(args.need_rerun_file)
    split_tests = load_lines_as_set(args.split_tests_file)

    prev_path = Path(args.prev_result)
    if not prev_path.exists():
        return 0

    try:
        prev = json.loads(prev_path.read_text())
    except (json.JSONDecodeError, ValueError):
        return 0

    carried = 0
    with open(args.out_jsonl, "a") as out:
        for entry in flatten_previous_results(prev):
            if entry.get("file") not in split_tests:
                continue
            if entry.get("file") in need_rerun:
                continue
            if entry.get("environment") != args.env:
                continue
            out.write(json.dumps(entry, ensure_ascii=False) + "\n")
            carried += 1

    print(f"Carried over {carried} results from previous run")
    return 0


def cmd_finalize(args: argparse.Namespace) -> int:
    items: list[Any] = []
    in_path = Path(args.in_jsonl)
    if in_path.exists():
        with open(in_path) as fh:
            for line in fh:
                line = line.strip()
                if not line:
                    continue
                items.append(json.loads(line))

    out_path = Path(args.out_json)
    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_text(json.dumps(items, indent=2, ensure_ascii=False) + "\n")
    print(f"Results written to {out_path} ({len(items)} tests)")
    return 0


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest="command", required=True)

    carry = subparsers.add_parser("carry-over")
    carry.add_argument("--prev-result", required=True)
    carry.add_argument("--need-rerun-file", required=True)
    carry.add_argument("--split-tests-file", required=True)
    carry.add_argument("--env", required=True)
    carry.add_argument("--out-jsonl", required=True)
    carry.set_defaults(func=cmd_carry_over)

    finalize = subparsers.add_parser("finalize")
    finalize.add_argument("--in-jsonl", required=True)
    finalize.add_argument("--out-json", required=True)
    finalize.set_defaults(func=cmd_finalize)

    return parser


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()
    return args.func(args)


if __name__ == "__main__":
    raise SystemExit(main())
