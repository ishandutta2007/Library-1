#!/usr/bin/env python3
"""テスト結果をマージして results.json を生成する (merge-results.ts の Python 版)

使い方:
  python3 scripts/merge-results.py                          # .cache/results/ からマージ
  python3 scripts/merge-results.py --prev .verify-results/results.json  # 前回結果とマージ
  python3 scripts/merge-results.py --from-committed         # .verify-results/results.json をそのまま利用
"""
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from lib.merge_results import merge_results

ROOT = Path(__file__).resolve().parent.parent
OUTPUT = ROOT / ".verify-results" / "results.json"


def main() -> None:
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument("--prev", help="前回結果ファイルのパス")
    parser.add_argument(
        "--from-committed",
        action="store_true",
        help="コミット済み結果をそのまま利用",
    )
    args = parser.parse_args()

    if args.from_committed:
        if OUTPUT.exists():
            print(f"Results available at {OUTPUT}")
        else:
            print("No committed results found")
        return

    compact = merge_results(root=ROOT, prev_file=args.prev)

    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    OUTPUT.write_text(json.dumps(compact, indent=2, ensure_ascii=False) + "\n")

    test_count = len(compact["tests"])
    hpp_count = len(compact["hpp_map"])
    print(f"\nResults: {test_count} test files, {hpp_count} hpp files")
    print(f"Written to {OUTPUT}")


if __name__ == "__main__":
    main()
