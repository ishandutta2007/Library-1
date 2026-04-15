"""テスト結果のマージ (merge-results.ts の Python 移植)

複数環境の result-*.json を前回結果とマージし、compact な results.json を生成する。
"""

import json
import os
import re
from datetime import datetime, timezone
from pathlib import Path

from lib.dependency_graph import build_dependency_graph, build_test_map
from lib.run_results import load_compact_results_from_path

ROOT = Path(__file__).resolve().parent.parent.parent


def _load_new_results(root: Path) -> list[dict]:
    result_dir = root / ".cache" / "results"
    new_results: list[dict] = []
    if not result_dir.exists():
        return new_results
    for f in sorted(result_dir.iterdir()):
        if f.name.startswith("result-") and f.name.endswith(".json"):
            data = json.loads(f.read_text())
            new_results.extend(data)
            print(f"Loaded {len(data)} results from {f.name}")
    return new_results


def _load_previous_tests(prev_path: str) -> dict:
    compact = load_compact_results_from_path(prev_path)
    tests = compact.get("tests", {})
    if tests:
        print(f"Loaded previous results from {prev_path}")
    return {k: dict(v) for k, v in tests.items()}


def _merge_tests(
    previous: dict[str, dict], new_results: list[dict]
) -> dict[str, dict]:
    merged: dict[str, dict] = {}
    for file, prev in previous.items():
        merged[file] = {**prev, "environments": {**prev.get("environments", {})}}

    for result in new_results:
        key = result["file"]
        if key not in merged:
            merged[key] = {
                "problem": result.get("problem", ""),
                "time_limit_ms": 0,
                "environments": {},
            }
        if result.get("problem"):
            merged[key]["problem"] = result["problem"]

        cases = result.get("cases", [])
        time_max = max((c["time_ms"] for c in cases), default=0)
        time_total = sum(c["time_ms"] for c in cases)
        mem_max = max((c["memory_kb"] for c in cases), default=0)

        env_summary: dict = {
            "status": result["status"],
            "summary": {
                "time_max_ms": time_max,
                "time_total_ms": time_total,
                "memory_max_kb": mem_max,
            },
            "last_execution_time": result.get("last_execution_time")
            or datetime.now(timezone.utc).isoformat(),
            "cases": cases,
        }
        if result.get("compile_error"):
            env_summary["compile_error"] = result["compile_error"]
        merged[key]["environments"][result["environment"]] = env_summary

    return merged


def _is_ignore_test(file_path: Path) -> bool:
    if not file_path.exists():
        return False
    content = file_path.read_text()
    return bool(re.search(r"competitive-verifier:\s*IGNORE", content))


_IGNORE_ENV_SUMMARY = {
    "status": "IGNORE",
    "summary": {"time_max_ms": 0, "time_total_ms": 0, "memory_max_kb": 0},
    "cases": [],
}


def _collect_env_names(tests: dict[str, dict]) -> list[str]:
    env_set: set[str] = set()
    for test in tests.values():
        env_set.update(test.get("environments", {}).keys())
    return sorted(env_set)


def _build_hpp_map(
    root: Path, tests: dict[str, dict]
) -> dict[str, list[str]]:
    graph = build_dependency_graph()
    hpp_test_map = build_test_map(graph)
    hpp_map: dict[str, list[str]] = {}
    env_names = _collect_env_names(tests)

    for hpp, test_files in hpp_test_map.items():
        files: list[str] = []
        for file in test_files:
            if file not in tests and _is_ignore_test(root / file):
                tests[file] = {
                    "problem": "",
                    "time_limit_ms": 0,
                    "environments": {
                        env: dict(_IGNORE_ENV_SUMMARY) for env in env_names
                    },
                }
            files.append(file)
        if files:
            hpp_map[hpp] = files

    return hpp_map


def merge_results(
    *, root: Path, prev_file: str | None = None
) -> dict:
    output_path = root / ".verify-results" / "results.json"
    new_results = _load_new_results(root)
    prev_path = prev_file or str(output_path)
    prev_tests = _load_previous_tests(prev_path) if os.path.exists(prev_path) else {}
    tests = _merge_tests(prev_tests, new_results)
    hpp_map = _build_hpp_map(root, tests)

    return {"tests": tests, "hpp_map": hpp_map}
