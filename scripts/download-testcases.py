#!/usr/bin/env python3
# /// script
# requires-python = ">=3.12"
# ///
"""
全テストファイルで必要なテストケースを一括ダウンロードする

テストケースの取得優先順位:
1. .cache/testcases/<md5>/ にキャッシュ済み (actions/cache)
2. tc.zip (手動で用意したもの、oj 非対応サイト用)
3. 各サービスの API からダウンロード

yosupo judge は library-checker-problems リポジトリから生成する方式。
"""
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from lib.download import (
    DownloadConfig,
    copy_checker_assets,
    download_batch,
    is_cached,
    url_to_md5,
)

ROOT = Path(__file__).resolve().parent.parent
TEST_DIR = ROOT / "test"
TC_ZIP_DIR = ROOT / ".cache" / "tc"
TC_CACHE_DIR = ROOT / ".cache" / "testcases"
TC_RESOLVED_DIR = ROOT / ".cache" / "tc-resolved"

YUKICODER_TOKEN = os.environ.get("YUKICODER_TOKEN", "")

CONFIG = DownloadConfig(
    tc_cache_dir=TC_CACHE_DIR,
    library_checker_dir=ROOT / ".cache" / "library-checker-problems",
    yukicoder_token=YUKICODER_TOKEN,
)


def get_problem_urls(split_file: str | None = None) -> dict[str, list[str]]:
    """テストファイルから PROBLEM URL を収集して重複排除。
    split_file が指定されていれば、そのファイルに列挙されたテストのみ対象。"""
    if split_file:
        test_files = []
        with open(split_file) as f:
            for line in f:
                line = line.strip()
                if line:
                    test_files.append(ROOT / line)
    else:
        test_files = sorted(TEST_DIR.rglob("*.test.cpp"))

    urls: dict[str, list[str]] = {}
    for test_file in test_files:
        if not test_file.exists():
            continue
        content = test_file.read_text()
        if re.search(r"competitive-verifier:\s*IGNORE", content):
            continue
        if re.search(r"competitive-verifier:\s*STANDALONE", content):
            continue
        m = re.search(r"competitive-verifier:\s*PROBLEM\s+(\S+)", content)
        if m:
            url = m.group(1)
            if url not in urls:
                urls[url] = []
            urls[url].append(str(test_file.relative_to(ROOT)))
    return urls


def copy_managed_checkers() -> int:
    """test/**/checkers/*.checker.cpp を該当 md5 ディレクトリに checker.cpp としてコピー。

    対応する PROBLEM URL は checker ファイル先頭の `competitive-verifier: PROBLEM <url>`
    から読み取る。コピー先は API キャッシュ (.cache/testcases/<md5>) を優先し、
    なければ tc.zip 展開先 (.cache/tc-resolved/<md5>) を使う。
    どちらも存在しないものはスキップ (まだダウンロード前)。
    """
    copied = 0
    checker_files = sorted(TEST_DIR.rglob("checkers/*.checker.cpp"))
    for checker in checker_files:
        content = checker.read_text()
        m = re.search(r"competitive-verifier:\s*PROBLEM\s+(\S+)", content)
        if not m:
            print(f"  [WARN] no PROBLEM URL in {checker.relative_to(ROOT)}")
            continue
        url = m.group(1)
        md5 = url_to_md5(url)
        candidates = [TC_CACHE_DIR / md5, TC_RESOLVED_DIR / md5]
        for dest in candidates:
            if dest.exists():
                shutil.copy2(checker, dest / "checker.cpp")
                copied += 1
                break
        else:
            print(
                f"  [SKIP] checker for {url}: no testcase dir yet "
                f"({checker.relative_to(ROOT)})"
            )
    return copied


def is_cached_any(url: str) -> bool:
    """API キャッシュまたは tc.zip 展開済みか判定"""
    if is_cached(url, CONFIG):
        return True
    resolved_dir = TC_RESOLVED_DIR / url_to_md5(url)
    return resolved_dir.exists() and any(resolved_dir.iterdir())


def try_tc_zip(url: str) -> bool:
    """tc.zip からテストケースをコピー（キャッシュ対象外の別ディレクトリに置く）"""
    md5 = url_to_md5(url)
    resolved_dir = TC_RESOLVED_DIR / md5
    if resolved_dir.exists() and any(resolved_dir.iterdir()):
        return True
    for problem_dir in [TC_ZIP_DIR / "tc" / md5, TC_ZIP_DIR / md5]:
        test_dir = problem_dir / "test"
        if test_dir.exists() and any(test_dir.iterdir()):
            resolved_dir.mkdir(parents=True, exist_ok=True)
            subprocess.run(
                ["cp", "-r", f"{test_dir}/.", str(resolved_dir)], check=True
            )
            copy_checker_assets(problem_dir, resolved_dir)
            return True
    return False


def main() -> None:
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument("--split", help="対象テストファイル一覧のファイルパス")
    args = parser.parse_args()

    urls = get_problem_urls(split_file=args.split)
    print(f"Found {len(urls)} unique problem URLs")

    # 1. tc.zip チェック (Library 固有)
    from_zip = 0
    urls_for_download: dict[str, list[str]] = {}
    for url, files in urls.items():
        if is_cached_any(url):
            pass  # download_batch 内で cached カウント
        elif try_tc_zip(url):
            from_zip += 1
        urls_for_download[url] = files

    if from_zip > 0:
        print(f"  From tc.zip: {from_zip}")

    # 2. API ダウンロード (共通モジュール)
    if not urls_for_download:
        print("No problem URLs found")
        return

    summary = download_batch(urls_for_download, CONFIG)

    # 3. test/**/checkers/*.checker.cpp を該当キャッシュディレクトリにコピー
    print("\nCopying managed checkers...")
    copied = copy_managed_checkers()
    print(f"  Copied: {copied}")

    print(f"\nTestcase download summary:")
    print(f"  Cached:      {summary.cached}")
    print(f"  From tc.zip: {from_zip}")
    print(f"  Downloaded:  {summary.downloaded}")
    print(f"  Failed:      {summary.failed}")
    print(f"  Total:       {summary.total}")
    print(f"  Checkers:    {copied}")


if __name__ == "__main__":
    main()
