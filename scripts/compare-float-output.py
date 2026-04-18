#!/usr/bin/env python3
# /// script
# requires-python = ">=3.12"
# ///
"""浮動小数点出力の比較 CLI

Usage:
    python3 scripts/compare-float-output.py --actual FILE --expected FILE --tolerance TOL
    Exit 0 = match, Exit 1 = mismatch
"""
import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from lib.float_compare import compare_float_outputs


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--actual", required=True)
    parser.add_argument("--expected", required=True)
    parser.add_argument("--tolerance", required=True, type=float)
    args = parser.parse_args()

    if not args.tolerance >= 0:
        print("Tolerance must be a non-negative finite number", file=sys.stderr)
        sys.exit(1)

    ok = compare_float_outputs(args.actual, args.expected, args.tolerance)
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
