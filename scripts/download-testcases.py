#!/usr/bin/env python3
"""
全テストファイルで必要なテストケースを一括ダウンロードする
oj コマンドに依存せず、各サービスの API を直接叩く

対応サービス:
- AOJ (onlinejudge.u-aizu.ac.jp) — 公開 API
- yosupo judge (judge.yosupo.jp) — library-checker-problems リポジトリからテストケース生成
- yukicoder — API + Bearer トークン
- HackerRank — 公開 API

テストケースの取得元:
1. キャッシュ済み (.cache/testcases/<md5>/)
2. tc.zip (oj 非対応サイト用、.cache/tc/ に展開済み前提)
3. 各サービスの API からダウンロード
"""
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import time
import urllib.request
import urllib.error
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TEST_DIR = ROOT / "test"
TC_ZIP_DIR = ROOT / ".cache" / "tc"
TC_CACHE_DIR = ROOT / ".cache" / "testcases"

YUKICODER_TOKEN = os.environ.get("YUKICODER_TOKEN", "")


def url_to_md5(url: str) -> str:
    return hashlib.md5(url.encode()).hexdigest()


def url_to_cache_dir(url: str) -> Path:
    return TC_CACHE_DIR / url_to_md5(url)


def get_problem_urls() -> dict[str, str]:
    """全テストファイルから PROBLEM URL を収集して重複排除"""
    urls = {}
    for test_file in sorted(TEST_DIR.rglob("*.test.cpp")):
        content = test_file.read_text()
        if re.search(r"competitive-verifier:\s*IGNORE", content):
            continue
        if re.search(r"competitive-verifier:\s*STANDALONE", content):
            continue
        m = re.search(r"competitive-verifier:\s*PROBLEM\s+(\S+)", content)
        if m:
            url = m.group(1)
            if url not in urls:
                urls[url] = str(test_file.relative_to(ROOT))
    return urls


def try_tc_zip(url: str, cache_dir: Path) -> bool:
    """tc.zip からテストケースをコピー（checker.cpp があればそれも）"""
    md5 = url_to_md5(url)
    for problem_dir in [TC_ZIP_DIR / "tc" / md5, TC_ZIP_DIR / md5]:
        test_dir = problem_dir / "test"
        if test_dir.exists() and any(test_dir.iterdir()):
            cache_dir.mkdir(parents=True, exist_ok=True)
            # テストケースをコピー
            subprocess.run(["cp", "-r", f"{test_dir}/.", str(cache_dir)], check=True)
            # checker.cpp があればコピー
            checker = problem_dir / "checker.cpp"
            if checker.exists():
                shutil.copy2(checker, cache_dir / "checker.cpp")
            return True
    return False


# ============================================================
# AOJ
# ============================================================

def download_aoj(url: str, cache_dir: Path) -> bool:
    """AOJ のテストケースをダウンロード"""
    # URL から problem_id を抽出
    # https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_A → DSL_2_A
    # https://onlinejudge.u-aizu.ac.jp/problems/0355 → 0355
    m = re.search(r"/problems/(\w+)", url) or re.search(r"/(\w+)$", url)
    if not m:
        return False
    problem_id = m.group(1)

    try:
        # ヘッダー取得 (テストケース一覧)
        header_url = f"https://judgedat.u-aizu.ac.jp/testcases/{problem_id}/header"
        req = urllib.request.Request(header_url)
        with urllib.request.urlopen(req, timeout=30) as resp:
            headers = json.loads(resp.read())
    except Exception as e:
        print(f"    AOJ header failed: {e}", file=sys.stderr)
        return False

    cache_dir.mkdir(parents=True, exist_ok=True)
    count = 0

    for tc in headers.get("headers", []):
        serial = tc.get("serial")
        name = tc.get("name", f"case{serial}")
        if serial is None:
            continue

        try:
            # 入力
            in_url = f"https://judgedat.u-aizu.ac.jp/testcases/{problem_id}/{serial}/in"
            with urllib.request.urlopen(in_url, timeout=30) as resp:
                in_data = resp.read()
            # 出力
            out_url = f"https://judgedat.u-aizu.ac.jp/testcases/{problem_id}/{serial}/out"
            with urllib.request.urlopen(out_url, timeout=30) as resp:
                out_data = resp.read()

            (cache_dir / f"{name}.in").write_bytes(in_data)
            (cache_dir / f"{name}.out").write_bytes(out_data)
            count += 1
        except Exception:
            continue

    return count > 0


# ============================================================
# yosupo judge (Library Checker)
# ============================================================

LIBRARY_CHECKER_DIR = ROOT / ".cache" / "library-checker-problems"


def download_yosupo(url: str, cache_dir: Path) -> bool:
    """yosupo judge のテストケースを生成し、checker もコンパイル"""
    # https://judge.yosupo.jp/problem/point_add_range_sum → point_add_range_sum
    m = re.search(r"/problem/(\w+)", url)
    if not m:
        return False
    problem_id = m.group(1)

    # library-checker-problems リポジトリをクローン/更新
    if not LIBRARY_CHECKER_DIR.exists():
        print("    Cloning library-checker-problems...", file=sys.stderr)
        subprocess.run(
            ["git", "clone", "--depth=1",
             "https://github.com/yosupo06/library-checker-problems.git",
             str(LIBRARY_CHECKER_DIR)],
            check=True, capture_output=True
        )

    # 問題ディレクトリを探す
    problem_dir = None
    for p in LIBRARY_CHECKER_DIR.rglob("info.toml"):
        if p.parent.name == problem_id:
            problem_dir = p.parent
            break

    if not problem_dir:
        return False

    # テストケース生成
    try:
        subprocess.run(
            [sys.executable, str(LIBRARY_CHECKER_DIR / "generate.py"), str(problem_dir)],
            check=True, capture_output=True, timeout=300,
            cwd=LIBRARY_CHECKER_DIR,
        )
    except Exception as e:
        print(f"    yosupo generate failed: {e}", file=sys.stderr)
        return False

    # 生成されたテストケースをコピー
    in_dir = problem_dir / "in"
    out_dir = problem_dir / "out"
    if not in_dir.exists():
        return False

    cache_dir.mkdir(parents=True, exist_ok=True)
    count = 0
    for in_file in sorted(in_dir.glob("*.in")):
        out_file = out_dir / in_file.name.replace(".in", ".out")
        if out_file.exists():
            shutil.copy2(in_file, cache_dir / in_file.name)
            shutil.copy2(out_file, cache_dir / out_file.name)
            count += 1

    # checker のソースをキャッシュにコピー（verify ジョブ側でアーキテクチャに合わせてコンパイル）
    checker_cpp = problem_dir / "checker.cpp"
    if checker_cpp.exists():
        shutil.copy2(checker_cpp, cache_dir / "checker.cpp")
        # testlib.h も必要
        testlib_h = LIBRARY_CHECKER_DIR / "common" / "testlib.h"
        if testlib_h.exists():
            shutil.copy2(testlib_h, cache_dir / "testlib.h")

    return count > 0


# ============================================================
# yukicoder
# ============================================================

def download_yukicoder(url: str, cache_dir: Path) -> bool:
    """yukicoder のテストケースをダウンロード"""
    if not YUKICODER_TOKEN:
        return False

    # https://yukicoder.me/problems/no/1006 → 1006
    m = re.search(r"/problems/no/(\d+)", url)
    if not m:
        return False
    problem_no = m.group(1)

    try:
        # テストケース一覧取得
        api_url = f"https://yukicoder.me/api/v1/problems/no/{problem_no}/file/in"
        req = urllib.request.Request(api_url)
        req.add_header("Authorization", f"Bearer {YUKICODER_TOKEN}")
        with urllib.request.urlopen(req, timeout=30) as resp:
            in_names = resp.read().decode().strip().split("\n")
    except Exception as e:
        print(f"    yukicoder list failed: {e}", file=sys.stderr)
        return False

    cache_dir.mkdir(parents=True, exist_ok=True)
    count = 0

    for name in in_names:
        name = name.strip()
        if not name:
            continue
        try:
            # 入力
            in_url = f"https://yukicoder.me/api/v1/problems/no/{problem_no}/file/in/{name}"
            req = urllib.request.Request(in_url)
            req.add_header("Authorization", f"Bearer {YUKICODER_TOKEN}")
            with urllib.request.urlopen(req, timeout=30) as resp:
                in_data = resp.read()

            # 出力
            out_name = name  # yukicoder は in/out のファイル名が同じ
            out_url = f"https://yukicoder.me/api/v1/problems/no/{problem_no}/file/out/{out_name}"
            req = urllib.request.Request(out_url)
            req.add_header("Authorization", f"Bearer {YUKICODER_TOKEN}")
            with urllib.request.urlopen(req, timeout=30) as resp:
                out_data = resp.read()

            base = Path(name).stem
            (cache_dir / f"{base}.in").write_bytes(in_data)
            (cache_dir / f"{base}.out").write_bytes(out_data)
            count += 1
        except Exception:
            continue
        time.sleep(0.1)  # レートリミット対策

    return count > 0


# ============================================================
# HackerRank
# ============================================================

def download_hackerrank(url: str, cache_dir: Path) -> bool:
    """HackerRank のテストケースをダウンロード"""
    # HackerRank はシステムテストケースの公開 API がないので oj に依存していた
    # oj なしでは対応困難 → tc.zip に含まれている分だけで対応
    return False


# ============================================================
# メイン
# ============================================================

def download_one(url: str) -> bool:
    """URL に応じたダウンロード関数を呼ぶ"""
    cache_dir = url_to_cache_dir(url)

    if "onlinejudge.u-aizu.ac.jp" in url:
        return download_aoj(url, cache_dir)
    elif "judge.yosupo.jp" in url:
        return download_yosupo(url, cache_dir)
    elif "yukicoder.me" in url:
        return download_yukicoder(url, cache_dir)
    elif "hackerrank.com" in url:
        return download_hackerrank(url, cache_dir)
    elif "codeforces.com" in url:
        return False  # 非対応
    elif "cses.fi" in url:
        return False  # 非対応
    else:
        return False


def download_one_with_url(url: str) -> tuple[str, bool]:
    """並列実行用ラッパー。(url, 成功したか) を返す"""
    try:
        return (url, download_one(url))
    except Exception as e:
        print(f"    Error downloading {url}: {e}", file=sys.stderr)
        return (url, False)


def main():
    from concurrent.futures import ThreadPoolExecutor, as_completed

    urls = get_problem_urls()
    print(f"Found {len(urls)} unique problem URLs")

    cached = 0
    from_zip = 0
    downloaded = 0
    failed = 0

    # キャッシュ済み・tc.zip の処理（順次、高速）
    to_download: list[str] = []

    for url in urls:
        cache_dir = url_to_cache_dir(url)

        if cache_dir.exists() and any(cache_dir.iterdir()):
            cached += 1
            continue

        if try_tc_zip(url, cache_dir):
            from_zip += 1
            print(f"  [ZIP] {url}")
            continue

        to_download.append(url)

    if not to_download:
        print(f"\nTestcase download summary:")
        print(f"  Cached:     {cached}")
        print(f"  From zip:   {from_zip}")
        print(f"  Downloaded: 0")
        print(f"  Failed:     0")
        print(f"  Total:      {len(urls)}")
        return

    # yosupo judge はリポジトリクローンが必要なので先に1回だけ実行
    yosupo_urls = [u for u in to_download if "judge.yosupo.jp" in u]
    other_urls = [u for u in to_download if "judge.yosupo.jp" not in u]

    # yosupo: リポジトリクローンは1回だけ、テストケース生成は並列化
    if yosupo_urls:
        # 先にクローンだけ実行
        if not LIBRARY_CHECKER_DIR.exists():
            print("  Cloning library-checker-problems...")
            subprocess.run(
                ["git", "clone", "--depth=1",
                 "https://github.com/yosupo06/library-checker-problems.git",
                 str(LIBRARY_CHECKER_DIR)],
                check=True, capture_output=True
            )
            # generate.py の依存をインストール
            subprocess.run(
                [sys.executable, "-m", "pip", "install", "-q", "pyyaml", "toml"],
                capture_output=True
            )

    print(f"  Downloading {len(to_download)} problems ({len(yosupo_urls)} yosupo, {len(other_urls)} other)...")

    # AOJ, yukicoder 等は I/O バウンドなので並列ダウンロード
    if other_urls:
        with ThreadPoolExecutor(max_workers=8) as executor:
            futures = {executor.submit(download_one_with_url, url): url for url in other_urls}
            for future in as_completed(futures):
                url, success = future.result()
                if success:
                    downloaded += 1
                    print(f"  [OK]  {url}")
                else:
                    failed += 1
                    print(f"  [NG]  {url}")

    # yosupo judge はリポジトリ内のファイルを操作するので順次実行
    for url in yosupo_urls:
        print(f"  [GEN] {url} ...", end="", flush=True)
        if download_one(url):
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
    print(f"  Total:      {len(urls)}")


if __name__ == "__main__":
    main()
