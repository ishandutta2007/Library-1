#!/usr/bin/env python3
"""
全テストファイルで必要なテストケースを一括ダウンロードする
oj コマンドに依存せず、各サービスの API を直接叩く

テストケースの取得優先順位:
1. .cache/testcases/<md5>/ にキャッシュ済み (actions/cache)
2. tc.zip (手動で用意したもの、oj 非対応サイト用)
3. 各サービスの API からダウンロード

yosupo judge は library-checker-problems リポジトリから生成する方式。
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
from concurrent.futures import ThreadPoolExecutor, ProcessPoolExecutor, as_completed
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


def get_problem_urls(split_file: str | None = None) -> dict[str, list[str]]:
    """テストファイルから PROBLEM URL を収集して重複排除。
    split_file が指定されていれば、そのファイルに列挙されたテストのみ対象。"""
    # 対象テストファイルの決定
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


def is_cached(url: str) -> bool:
    cache_dir = url_to_cache_dir(url)
    return cache_dir.exists() and any(cache_dir.iterdir())


def try_tc_zip(url: str) -> bool:
    """tc.zip からテストケースをコピー（checker.cpp があればそれも）"""
    md5 = url_to_md5(url)
    cache_dir = url_to_cache_dir(url)
    for problem_dir in [TC_ZIP_DIR / "tc" / md5, TC_ZIP_DIR / md5]:
        test_dir = problem_dir / "test"
        if test_dir.exists() and any(test_dir.iterdir()):
            cache_dir.mkdir(parents=True, exist_ok=True)
            subprocess.run(["cp", "-r", f"{test_dir}/.", str(cache_dir)], check=True)
            checker = problem_dir / "checker.cpp"
            if checker.exists():
                shutil.copy2(checker, cache_dir / "checker.cpp")
            return True
    return False


# ============================================================
# AOJ
# ============================================================

def download_aoj(url: str) -> bool:
    """AOJ のテストケースをダウンロード"""
    cache_dir = url_to_cache_dir(url)
    m = re.search(r"/problems/(\w+)", url) or re.search(r"/(\w+)$", url)
    if not m:
        return False
    problem_id = m.group(1)

    try:
        header_url = f"https://judgedat.u-aizu.ac.jp/testcases/{problem_id}/header"
        with urllib.request.urlopen(header_url, timeout=30) as resp:
            headers = json.loads(resp.read())
    except urllib.error.HTTPError as e:
        print(f"    AOJ {problem_id}: HTTP {e.code} {e.reason}", flush=True)
        return False
    except Exception as e:
        print(f"    AOJ {problem_id}: {e}", flush=True)
        return False

    # 一時ディレクトリで作業し、全ケース成功したらキャッシュにリネーム
    tmp_dir = Path(str(cache_dir) + ".tmp")
    if tmp_dir.exists():
        shutil.rmtree(tmp_dir)
    tmp_dir.mkdir(parents=True, exist_ok=True)

    expected = [tc for tc in headers.get("headers", []) if tc.get("serial") is not None]
    count = 0

    for tc in expected:
        serial = tc["serial"]
        name = tc.get("name", f"case{serial}")
        try:
            in_url = f"https://judgedat.u-aizu.ac.jp/testcases/{problem_id}/{serial}/in"
            with urllib.request.urlopen(in_url, timeout=30) as resp:
                in_data = resp.read()
            out_url = f"https://judgedat.u-aizu.ac.jp/testcases/{problem_id}/{serial}/out"
            with urllib.request.urlopen(out_url, timeout=30) as resp:
                out_data = resp.read()
            (tmp_dir / f"{name}.in").write_bytes(in_data)
            (tmp_dir / f"{name}.out").write_bytes(out_data)
            count += 1
        except Exception:
            # 1ケースでも失敗したら全部破棄
            shutil.rmtree(tmp_dir)
            return False

    if count > 0:
        if cache_dir.exists():
            shutil.rmtree(cache_dir)
        tmp_dir.rename(cache_dir)
        return True
    else:
        shutil.rmtree(tmp_dir)
        return False


# ============================================================
# yosupo judge (Library Checker)
# ============================================================

LIBRARY_CHECKER_DIR = ROOT / ".cache" / "library-checker-problems"


def ensure_library_checker_repo():
    """library-checker-problems リポジトリをクローンまたは更新"""
    if LIBRARY_CHECKER_DIR.exists():
        # 既にあれば最新に更新
        print("  Updating library-checker-problems...", flush=True)
        try:
            subprocess.run(
                ["git", "fetch", "--depth=1", "origin", "master"],
                check=True, capture_output=True, timeout=60,
                cwd=LIBRARY_CHECKER_DIR,
            )
            subprocess.run(
                ["git", "reset", "--hard", "origin/master"],
                check=True, capture_output=True, timeout=30,
                cwd=LIBRARY_CHECKER_DIR,
            )
            return True
        except Exception as e:
            print(f"  Update failed: {e}", flush=True)
            # 更新失敗しても既存のリポジトリで続行
            return True
    else:
        print("  Cloning library-checker-problems...", flush=True)
        try:
            subprocess.run(
                ["git", "clone", "--depth=1",
                 "https://github.com/yosupo06/library-checker-problems.git",
                 str(LIBRARY_CHECKER_DIR)],
                check=True, capture_output=True, timeout=120,
            )
        except Exception as e:
            print(f"  Clone failed: {e}", flush=True)
            return False

    # generate.py の依存をインストール
    subprocess.run(
        [sys.executable, "-m", "pip", "install", "-q", "pyyaml", "toml"],
        capture_output=True,
    )
    return True


def download_yosupo(url: str) -> bool:
    """yosupo judge のテストケースを生成し、checker もコピー"""
    cache_dir = url_to_cache_dir(url)
    m = re.search(r"/problem/(\w+)", url)
    if not m:
        return False
    problem_id = m.group(1)

    if not ensure_library_checker_repo():
        return False

    # 問題ディレクトリを探す
    problem_dir = None
    for p in LIBRARY_CHECKER_DIR.rglob("info.toml"):
        if p.parent.name == problem_id:
            problem_dir = p.parent
            break

    if not problem_dir:
        return False

    # テストケース生成 (generate.py には info.toml のパスを渡す)
    try:
        subprocess.run(
            [sys.executable, str(LIBRARY_CHECKER_DIR / "generate.py"),
             str(problem_dir / "info.toml")],
            check=True, timeout=300,
            cwd=LIBRARY_CHECKER_DIR,
        )
    except Exception as e:
        print(f"    yosupo generate failed for {problem_id}: {e}", flush=True)
        return False

    # 生成されたテストケースを一時ディレクトリにコピー
    in_dir = problem_dir / "in"
    out_dir = problem_dir / "out"
    if not in_dir.exists():
        return False

    tmp_dir = Path(str(cache_dir) + ".tmp")
    if tmp_dir.exists():
        shutil.rmtree(tmp_dir)
    tmp_dir.mkdir(parents=True, exist_ok=True)

    count = 0
    for in_file in sorted(in_dir.glob("*.in")):
        out_file = out_dir / in_file.name.replace(".in", ".out")
        if out_file.exists():
            shutil.copy2(in_file, tmp_dir / in_file.name)
            shutil.copy2(out_file, tmp_dir / out_file.name)
            count += 1

    # checker のソースをコピー
    checker_cpp = problem_dir / "checker.cpp"
    if checker_cpp.exists():
        shutil.copy2(checker_cpp, tmp_dir / "checker.cpp")
        testlib_h = LIBRARY_CHECKER_DIR / "common" / "testlib.h"
        if testlib_h.exists():
            shutil.copy2(testlib_h, tmp_dir / "testlib.h")

    if count > 0:
        if cache_dir.exists():
            shutil.rmtree(cache_dir)
        tmp_dir.rename(cache_dir)
        return True
    else:
        shutil.rmtree(tmp_dir)
        return False


# ============================================================
# yukicoder
# ============================================================

def download_yukicoder(url: str) -> bool:
    """yukicoder のテストケースをダウンロード（testcase.zip 一括取得）"""
    import zipfile
    import io

    cache_dir = url_to_cache_dir(url)
    if not YUKICODER_TOKEN:
        print(f"    yukicoder: no token set", flush=True)
        return False

    m = re.search(r"/problems/no/(\d+)", url)
    if not m:
        print(f"    yukicoder: cannot parse problem number from {url}", flush=True)
        return False
    problem_no = m.group(1)

    # testcase.zip を一括ダウンロード
    zip_url = f"https://yukicoder.me/problems/no/{problem_no}/testcase.zip"
    req = urllib.request.Request(zip_url)
    req.add_header("Authorization", f"Bearer {YUKICODER_TOKEN}")

    try:
        with urllib.request.urlopen(req, timeout=60) as resp:
            zip_data = resp.read()
    except urllib.error.HTTPError as e:
        print(f"    yukicoder #{problem_no}: HTTP {e.code} {e.reason}", flush=True)
        return False
    except Exception as e:
        print(f"    yukicoder #{problem_no}: {e}", flush=True)
        return False

    # zip を展開
    tmp_dir = Path(str(cache_dir) + ".tmp")
    if tmp_dir.exists():
        shutil.rmtree(tmp_dir)
    tmp_dir.mkdir(parents=True, exist_ok=True)

    try:
        with zipfile.ZipFile(io.BytesIO(zip_data)) as zf:
            # test_in/xxx.txt, test_out/xxx.txt の構造
            inputs = {}
            outputs = {}
            for name in zf.namelist():
                if name.startswith("test_in/") and not name.endswith("/"):
                    case_name = Path(name).stem
                    inputs[case_name] = zf.read(name)
                elif name.startswith("test_out/") and not name.endswith("/"):
                    case_name = Path(name).stem
                    outputs[case_name] = zf.read(name)

            count = 0
            for case_name in inputs:
                if case_name in outputs:
                    (tmp_dir / f"{case_name}.in").write_bytes(inputs[case_name])
                    (tmp_dir / f"{case_name}.out").write_bytes(outputs[case_name])
                    count += 1
    except Exception as e:
        print(f"    yukicoder #{problem_no}: zip extract failed: {e}", flush=True)
        shutil.rmtree(tmp_dir)
        return False

    if count > 0:
        if cache_dir.exists():
            shutil.rmtree(cache_dir)
        tmp_dir.rename(cache_dir)
        return True
    else:
        shutil.rmtree(tmp_dir)
        return False


# ============================================================
# ディスパッチ
# ============================================================

def download_hackerrank(url: str) -> bool:
    """HackerRank のテストケースをダウンロード（zip 形式）"""
    import zipfile
    import io

    cache_dir = url_to_cache_dir(url)

    # URL からコンテストとチャレンジを抽出
    # https://www.hackerrank.com/contests/w33/challenges/bonnie-and-clyde → w33, bonnie-and-clyde
    # https://www.hackerrank.com/challenges/morgan-and-a-string → master, morgan-and-a-string
    m = re.search(r"hackerrank\.com/contests/([^/]+)/challenges/([^/]+)", url)
    if m:
        contest, challenge = m.group(1), m.group(2)
    else:
        m = re.search(r"hackerrank\.com/challenges/([^/]+)", url)
        if not m:
            return False
        contest, challenge = "master", m.group(1)

    api_url = f"https://www.hackerrank.com/rest/contests/{contest}/challenges/{challenge}/download_testcases"
    req = urllib.request.Request(api_url)
    req.add_header("User-Agent", "Mozilla/5.0")

    try:
        with urllib.request.urlopen(req, timeout=60) as resp:
            zip_data = resp.read()
    except urllib.error.HTTPError as e:
        print(f"    HackerRank {challenge}: HTTP {e.code} {e.reason}", flush=True)
        return False
    except Exception as e:
        print(f"    HackerRank {challenge}: {e}", flush=True)
        return False

    tmp_dir = Path(str(cache_dir) + ".tmp")
    if tmp_dir.exists():
        shutil.rmtree(tmp_dir)
    tmp_dir.mkdir(parents=True, exist_ok=True)

    try:
        with zipfile.ZipFile(io.BytesIO(zip_data)) as zf:
            count = 0
            for name in zf.namelist():
                # input/input00.txt, output/output00.txt の形式
                if name.startswith("input/") and name.endswith(".txt"):
                    case_name = Path(name).stem  # input00
                    in_data = zf.read(name)
                    out_name = name.replace("input/", "output/").replace("input", "output")
                    if out_name in zf.namelist():
                        out_data = zf.read(out_name)
                        (tmp_dir / f"{case_name}.in").write_bytes(in_data)
                        (tmp_dir / f"{case_name}.out").write_bytes(out_data)
                        count += 1
    except Exception:
        shutil.rmtree(tmp_dir)
        return False

    if count > 0:
        if cache_dir.exists():
            shutil.rmtree(cache_dir)
        tmp_dir.rename(cache_dir)
        return True
    else:
        shutil.rmtree(tmp_dir)
        return False


def download_one(url: str) -> bool:
    """URL に応じたダウンロード関数を呼ぶ"""
    if "onlinejudge.u-aizu.ac.jp" in url:
        return download_aoj(url)
    elif "judge.yosupo.jp" in url:
        return download_yosupo(url)
    elif "yukicoder.me" in url:
        return download_yukicoder(url)
    elif "hackerrank.com" in url:
        return download_hackerrank(url)
    else:
        return False


def download_one_safe(url: str) -> tuple[str, bool]:
    """並列実行用ラッパー"""
    try:
        return (url, download_one(url))
    except Exception as e:
        print(f"    Error: {url}: {e}", file=sys.stderr)
        return (url, False)


# ============================================================
# メイン
# ============================================================

def main():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("--split", help="対象テストファイル一覧のファイルパス")
    args = parser.parse_args()

    urls = get_problem_urls(split_file=args.split)
    print(f"Found {len(urls)} unique problem URLs")

    cached = 0
    from_zip = 0
    to_download: list[str] = []

    # 1. キャッシュ済み / tc.zip から取得
    for url in urls:
        if is_cached(url):
            cached += 1
            continue
        if try_tc_zip(url):
            from_zip += 1
            continue
        to_download.append(url)

    print(f"  Cached: {cached}, From tc.zip: {from_zip}")
    print(f"  Need download: {len(to_download)}")

    if not to_download:
        print("All testcases available!")
        return

    # 2. API からダウンロード
    yosupo_urls = [u for u in to_download if "judge.yosupo.jp" in u]
    other_urls = [u for u in to_download if "judge.yosupo.jp" not in u]

    downloaded = 0
    failed = 0

    # AOJ, yukicoder 等は並列ダウンロード
    if other_urls:
        print(f"  Downloading {len(other_urls)} problems (parallel)...")
        with ThreadPoolExecutor(max_workers=8) as executor:
            futures = {executor.submit(download_one_safe, url): url for url in other_urls}
            for future in as_completed(futures):
                url, success = future.result()
                if success:
                    downloaded += 1
                    print(f"  [OK]  {url}")
                else:
                    failed += 1
                    print(f"  [NG]  {url}")

    # yosupo はプロセス並列で生成（問題ごとに独立したディレクトリに出力するので安全）
    if yosupo_urls:
        # 先にリポジトリのクローン/更新と依存インストールを済ませる
        if not ensure_library_checker_repo():
            print("  Failed to prepare library-checker-problems, skipping yosupo")
            failed += len(yosupo_urls)
        else:
            print(f"  Generating {len(yosupo_urls)} yosupo problems (parallel)...")
            with ProcessPoolExecutor(max_workers=3) as executor:
                futures = {executor.submit(download_one_safe, url): url for url in yosupo_urls}
                for future in as_completed(futures):
                    url, success = future.result()
                    m = re.search(r"/problem/(\w+)", url)
                    name = m.group(1) if m else url
                    if success:
                        downloaded += 1
                        print(f"  [GEN] {name} ... OK", flush=True)
                    else:
                        failed += 1
                        print(f"  [GEN] {name} ... FAILED", flush=True)

    print(f"\nTestcase download summary:")
    print(f"  Cached:      {cached}")
    print(f"  From tc.zip: {from_zip}")
    print(f"  Downloaded:  {downloaded}")
    print(f"  Failed:      {failed}")
    print(f"  Total:       {len(urls)}")


if __name__ == "__main__":
    main()
