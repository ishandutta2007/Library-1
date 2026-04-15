"""浮動小数点出力の比較モジュール (float-compare.ts の Python 移植)

相対誤差+絶対誤差の両方を考慮したトークン単位の比較を行う。
stdlib のみ使用。
"""

import re

_FLOAT_RE = re.compile(
    r"^[+-]?(?:Infinity|(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?)$"
)


def _normalize_newlines(text: str) -> str:
    return text.replace("\r\n", "\n").replace("\r", "\n")


def _trim_trailing_newlines(text: str) -> str:
    return text.rstrip("\n")


def _split_lines_for_comparison(text: str) -> list[str]:
    return _trim_trailing_newlines(text).split("\n")


def _is_strict_float_token(token: str) -> bool:
    return _FLOAT_RE.match(token) is not None


def _try_parse_float(token: str) -> float | None:
    if not _is_strict_float_token(token):
        return None
    return float(token)


def is_close(actual: float, expected: float, tolerance: float) -> bool:
    """相対+絶対誤差の判定: difference <= max(tolerance, tolerance * scale)"""
    if actual == expected:
        return True
    difference = abs(actual - expected)
    scale = max(abs(actual), abs(expected))
    return difference <= max(tolerance, tolerance * scale)


def compare_float_text(actual: str, expected: str, tolerance: float) -> bool:
    """CRLF 正規化 → 行分割 → トークン分割 → is_close で比較"""
    actual = _normalize_newlines(actual)
    expected = _normalize_newlines(expected)

    if actual == expected:
        return True

    actual_lines = _split_lines_for_comparison(actual)
    expected_lines = _split_lines_for_comparison(expected)
    if len(actual_lines) != len(expected_lines):
        return False

    for actual_line, expected_line in zip(actual_lines, expected_lines):
        actual_words = actual_line.split()
        expected_words = expected_line.split()

        if len(actual_words) != len(expected_words):
            return False

        for actual_word, expected_word in zip(actual_words, expected_words):
            if actual_word == expected_word:
                continue

            actual_value = _try_parse_float(actual_word)
            expected_value = _try_parse_float(expected_word)
            if actual_value is None or expected_value is None:
                return False
            if not is_close(actual_value, expected_value, tolerance):
                return False

    return True


def compare_float_outputs(
    actual_path: str, expected_path: str, tolerance: float
) -> bool:
    """ファイル版の比較"""
    with open(actual_path) as f:
        actual = f.read()
    with open(expected_path) as f:
        expected = f.read()
    return compare_float_text(actual, expected, tolerance)
