#!/usr/bin/env python3
"""
全テストファイルで必要なテストケースを一括ダウンロードする
setup ジョブで1回だけ実行し、結果を artifact で各 verify ジョブに渡す

テストケースの取得元:
1. tc.zip (oj 非対応サイト用、.cache/tc/ に展開済み前提)
2. oj download --system (AOJ, yosupo, yukicoder, HackerRank 等)
"""
import hashlib
import os
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TEST_DIR = ROOT / "test"
TC_ZIP_DIR = ROOT / ".cache" / "tc"
TC_CACHE_DIR = ROOT / ".cache" / "testcases"

YUKICODER_TOKEN = os.environ.get("YUKICODER_TOKEN", "")
DROPBOX_TOKEN = os.environ.get("DROPBOX_TOKEN", "")


def get_problem_urls() -> dict[str, str]:
    """全テストファイルから PROBLEM URL を収集して重複排除"""
    urls = {}
    for test_file in sorted(TEST_DIR.rglob("*.test.cpp")):
        content = test_file.read_text()

        # IGNORE はスキップ
        if re.search(r"competitive-verifier:\s*IGNORE", content):
            continue
        # STANDALONE はテストケース不要
        if re.search(r"competitive-verifier:\s*STANDALONE", content):
            continue

        m = re.search(r"competitive-verifier:\s*PROBLEM\s+(\S+)", content)
        if m:
            url = m.group(1)
            if url not in urls:
                urls[url] = str(test_file.relative_to(ROOT))
    return urls


def url_to_cache_dir(url: str) -> Path:
    """URL からキャッシュディレクトリパスを生成"""
    url_hash = hashlib.md5(url.encode()).hexdigest()
    return TC_CACHE_DIR / url_hash


def try_tc_zip(url: str, cache_dir: Path) -> bool:
    """tc.zip からテストケースをコピー"""
    url_md5 = hashlib.md5(url.encode()).hexdigest()

    # .cache/tc/tc/<md5>/test/ の構造
    for prefix in [TC_ZIP_DIR / "tc" / url_md5 / "test", TC_ZIP_DIR / url_md5 / "test"]:
        if prefix.exists() and any(prefix.iterdir()):
            cache_dir.mkdir(parents=True, exist_ok=True)
            subprocess.run(["cp", "-r", f"{prefix}/.", str(cache_dir)], check=True)
            return True
    return False


def try_oj_download(url: str, cache_dir: Path) -> bool:
    """oj download でテストケースを取得"""
    cache_dir.mkdir(parents=True, exist_ok=True)

    cmd = ["oj", "download", "--system", "-d", str(cache_dir), "-y"]

    # サービス固有のトークン
    if "yukicoder" in url and YUKICODER_TOKEN:
        cmd.extend(["--yukicoder-token", YUKICODER_TOKEN])
    if "atcoder" in url and DROPBOX_TOKEN:
        cmd.extend(["--dropbox-token", DROPBOX_TOKEN])

    cmd.append(url)

    try:
        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            timeout=120,
        )
        # テストケースが1つ以上あるか確認
        if any(cache_dir.rglob("*.in")) or any(cache_dir.rglob("in.txt")):
            return True
    except subprocess.TimeoutExpired:
        pass
    except Exception as e:
        print(f"  Error: {e}", file=sys.stderr)

    # 失敗した場合はディレクトリを削除
    import shutil
    if cache_dir.exists():
        shutil.rmtree(cache_dir)
    return False


def main():
    urls = get_problem_urls()
    print(f"Found {len(urls)} unique problem URLs")

    downloaded = 0
    cached = 0
    from_zip = 0
    failed = 0

    for url, example_file in urls.items():
        cache_dir = url_to_cache_dir(url)

        # 既にキャッシュがある
        if cache_dir.exists() and any(cache_dir.iterdir()):
            cached += 1
            continue

        # tc.zip から取得
        if try_tc_zip(url, cache_dir):
            from_zip += 1
            print(f"  [ZIP] {url}")
            continue

        # oj download
        print(f"  [DL] {url} ...", end="", flush=True)
        if try_oj_download(url, cache_dir):
            downloaded += 1
            print(" OK")
        else:
            failed += 1
            print(" FAILED")

    print(f"\nTestcase download summary:")
    print(f"  Cached:     {cached}")
    print(f"  From zip:   {from_zip}")
    print(f"  Downloaded: {downloaded}")
    print(f"  Failed:     {failed}")
    print(f"  Total:      {cached + from_zip + downloaded + failed}")


if __name__ == "__main__":
    main()
