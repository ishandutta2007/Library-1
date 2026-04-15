"""float_compare.py のテスト (results-tools.test.ts のケースを移植)"""
import os
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
from lib.float_compare import compare_float_outputs, compare_float_text


class TestCompareFloatOutputs(unittest.TestCase):
    def test_respects_tolerance_and_token_count(self):
        with tempfile.TemporaryDirectory() as d:
            actual = os.path.join(d, "actual.txt")
            expected = os.path.join(d, "expected.txt")

            Path(actual).write_text("2.0\n3.0\n3.999999999\n")
            Path(expected).write_text("2.0\n3.0\n4.0\n")

            self.assertTrue(compare_float_outputs(actual, expected, 1e-8))
            self.assertFalse(compare_float_outputs(actual, expected, 1e-12))

            Path(actual).write_text("1 2\n")
            Path(expected).write_text("1\n")
            self.assertFalse(compare_float_outputs(actual, expected, 1e-8))


class TestCompareFloatText(unittest.TestCase):
    def test_mixed_string_and_float(self):
        self.assertTrue(
            compare_float_text("OK 3.14\n", "OK 3.140000001\n", 1e-6)
        )
        self.assertTrue(compare_float_text("NA\n", "NA\n", 1e-8))
        self.assertFalse(compare_float_text("NA\n", "0 0\n", 1e-8))

    def test_relative_tolerance_for_large_values(self):
        self.assertTrue(
            compare_float_text("1000000.0\n", "1000000.5\n", 1e-6)
        )
        self.assertFalse(
            compare_float_text("1000000.0\n", "1000002.0\n", 1e-6)
        )

    def test_rejects_partial_numeric_parsing_and_shape_mismatches(self):
        self.assertFalse(compare_float_text("1abc\n", "1\n", 1e-6))
        self.assertFalse(
            compare_float_text("A 1\nB\n", "A 1\nB 2\n", 1e-6)
        )

    def test_normalizes_newlines_and_ignores_trailing(self):
        self.assertTrue(
            compare_float_text("X 1.0\r\nY 2.0\r\n", "X 1\nY 2\n\n", 1e-8)
        )


if __name__ == "__main__":
    unittest.main()
