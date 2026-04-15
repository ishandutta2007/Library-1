"""run_results.py のテスト (results-tools.test.ts のケースを移植)"""
import json
import os
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
from lib.run_results import (
    CASE_RECORD_SEPARATOR,
    build_result_entry,
    carry_over_results,
    finalize_results,
)


class TestBuildResultEntry(unittest.TestCase):
    def test_parses_case_record_file(self):
        with tempfile.TemporaryDirectory() as d:
            cases_file = os.path.join(d, "cases.txt")
            lines = [
                CASE_RECORD_SEPARATOR.join(["case1", "AC", "12", "34", ""]),
                CASE_RECORD_SEPARATOR.join(
                    ["case2", "WA", "56", "78", "detail text"]
                ),
            ]
            Path(cases_file).write_text("\n".join(lines) + "\n")

            entry = build_result_entry(
                file="test/foo.test.cpp",
                problem="https://example.com/foo",
                environment="x64-g++",
                status="WA",
                last_execution_time="2026-04-14T00:00:00+00:00",
                cases_records=cases_file,
            )

            self.assertEqual(len(entry["cases"]), 2)
            self.assertEqual(
                entry["cases"][0],
                {"name": "case1", "status": "AC", "time_ms": 12, "memory_kb": 34},
            )
            self.assertEqual(entry["cases"][1]["detail"], "detail text")


class TestCarryOverAndFinalize(unittest.TestCase):
    def test_keep_only_cached_target_env_results(self):
        with tempfile.TemporaryDirectory() as d:
            prev_file = os.path.join(d, "prev.json")
            need_rerun_file = os.path.join(d, "need-rerun.txt")
            split_tests_file = os.path.join(d, "split-tests.txt")
            jsonl_file = os.path.join(d, "result.jsonl")
            out_json_file = os.path.join(d, "result.json")

            prev_data = {
                "tests": {
                    "test/a.test.cpp": {
                        "problem": "A",
                        "time_limit_ms": 1000,
                        "environments": {
                            "x64-g++": {
                                "status": "AC",
                                "last_execution_time": "2026-04-14T00:00:00+00:00",
                                "summary": {
                                    "time_max_ms": 1,
                                    "time_total_ms": 1,
                                    "memory_max_kb": 1,
                                },
                                "cases": [],
                            },
                            "x64-clang++": {
                                "status": "WA",
                                "last_execution_time": "2026-04-14T00:00:00+00:00",
                                "summary": {
                                    "time_max_ms": 2,
                                    "time_total_ms": 2,
                                    "memory_max_kb": 2,
                                },
                                "cases": [],
                            },
                        },
                    },
                    "test/b.test.cpp": {
                        "problem": "B",
                        "time_limit_ms": 1000,
                        "environments": {
                            "x64-g++": {
                                "status": "AC",
                                "last_execution_time": "2026-04-14T00:00:00+00:00",
                                "summary": {
                                    "time_max_ms": 3,
                                    "time_total_ms": 3,
                                    "memory_max_kb": 3,
                                },
                                "cases": [],
                            },
                        },
                    },
                },
                "hpp_map": {},
            }
            Path(prev_file).write_text(json.dumps(prev_data, indent=2))
            Path(need_rerun_file).write_text("test/b.test.cpp\n")
            Path(split_tests_file).write_text(
                "test/a.test.cpp\ntest/b.test.cpp\n"
            )

            carried = carry_over_results(
                prev_result=prev_file,
                need_rerun_file=need_rerun_file,
                split_tests_file=split_tests_file,
                env="x64-g++",
                out_jsonl=jsonl_file,
            )
            self.assertEqual(carried, 1)

            count = finalize_results(
                in_jsonl=jsonl_file, out_json=out_json_file
            )
            self.assertEqual(count, 1)

            data = json.loads(Path(out_json_file).read_text())
            self.assertEqual(len(data), 1)
            self.assertEqual(data[0]["file"], "test/a.test.cpp")
            self.assertEqual(data[0]["environment"], "x64-g++")


if __name__ == "__main__":
    unittest.main()
