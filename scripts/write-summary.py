#!/usr/bin/env python3
"""GitHub Actions の Job Summary に検証結果のサマリーを出力する (write-summary.ts の Python 版)"""
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from lib.run_results import load_compact_results_from_path

ROOT = Path(__file__).resolve().parent.parent
RESULTS_PATH = ROOT / ".verify-results" / "results.json"


def load_split_map() -> dict[str, int]:
    """split-*.txt からテストファイル→split番号のマッピングを構築"""
    split_map: dict[str, int] = {}
    for i in range(100):
        path = Path(f"split-{i}.txt")
        if not path.exists():
            break
        for line in path.read_text().splitlines():
            trimmed = line.strip()
            if trimmed:
                split_map[trimmed] = i
    return split_map


def format_mem(kb: int) -> str:
    if kb >= 1024:
        return f"{kb / 1024:.1f}MB"
    return f"{kb}KB"


def main() -> None:
    data = load_compact_results_from_path(str(RESULTS_PATH))
    split_map = load_split_map()

    if not data["tests"]:
        print("No results found.")
        return

    passed = 0
    failed = 0
    skipped = 0
    ce = 0
    env_set: set[str] = set()

    rows: list[dict] = []
    for file in sorted(data["tests"]):
        result = data["tests"][file]
        env_results: dict[str, dict] = {}
        for env, env_result in result.get("environments", {}).items():
            env_set.add(env)
            summary = env_result.get("summary", {})
            env_results[env] = {
                "status": env_result.get("status", ""),
                "time_max": summary.get("time_max_ms", 0),
                "mem_max": summary.get("memory_max_kb", 0),
            }
        rows.append(
            {"file": file, "split": split_map.get(file), "env_results": env_results}
        )

    for row in rows:
        for info in row["env_results"].values():
            status = info["status"]
            if status == "AC":
                passed += 1
            elif status == "CE":
                ce += 1
            elif status in ("WA", "TLE", "MLE", "RE"):
                failed += 1
            else:
                skipped += 1

    total = passed + failed + ce + skipped
    envs = sorted(env_set)

    print("## Verification Result\n")
    if failed == 0 and ce == 0:
        print("✅ All test cases passed!\n")
    else:
        print("❌ Some test cases failed.\n")

    print("| | Count |")
    print("|:---|---:|")
    print(f"| ✅ Passed | {passed} |")
    print(f"| ❌ Failed | {failed} |")
    print(f"| 🔨 CE | {ce} |")
    print(f"| ⏭️ Skipped | {skipped} |")
    print(f"| **Total** | **{total}** |")
    print()

    # 失敗したテストの詳細
    failed_rows = [
        r
        for r in rows
        if any(
            info["status"] not in ("AC", "IGNORE")
            for info in r["env_results"].values()
        )
    ]

    if failed_rows:
        print("### Failed / Non-AC Tests\n")
        header = " | ".join(envs)
        print(f"| File | {header} | Job |")
        sep = "|".join(":---:" for _ in envs)
        print(f"|:---|{sep}|:---|")
        for row in failed_rows[:50]:
            cells: list[str] = []
            for env in envs:
                info = row["env_results"].get(env)
                if not info:
                    cells.append("-")
                elif info["status"] == "AC":
                    cells.append(
                        f"✅ {info['time_max']}ms {format_mem(info['mem_max'])}"
                    )
                elif info["status"] == "CE":
                    cells.append("🔨 CE")
                elif info["status"] == "IGNORE":
                    cells.append("⏭️")
                else:
                    cells.append(
                        f"❌ {info['status']} {info['time_max']}ms "
                        f"{format_mem(info['mem_max'])}"
                    )
            job_cell = f"split-{row['split']}" if row["split"] is not None else "-"
            print(f"| {row['file']} | {' | '.join(cells)} | {job_cell} |")
        if len(failed_rows) > 50:
            print(f"\n... and {len(failed_rows) - 50} more")


if __name__ == "__main__":
    main()
