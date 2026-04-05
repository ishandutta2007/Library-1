#!/usr/bin/env python3
"""
全テストファイルで必要なテストケースを一括ダウンロードする
oj コマンドに依存せず、各サービスの API を直接叩く

テストケースの取得優先順位:
1. .cache/testcases/<md5>/ にキャッシュ済み (actions/cache)
2. tc.zip (手動で用意したもの、oj 非対応サイト用)
3. tc-downloaded.zip (前回 CI でダウンロードしたもの、Releases に保存)
4. 各サービスの API からダウンロード

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
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TEST_DIR = ROOT / "test"
TC_ZIP_DIR = ROOT / ".cache" / "tc"
TC_DOWNLOADED_DIR = ROOT / ".cache" / "tc-downloaded"
TC_CACHE_DIR = ROOT / ".cache" / "testcases"

YUKICODER_TOKEN = os.environ.get("YUKICODER_TOKEN", "")


def url_to_md5(url: str) -> str:
    return hashlib.md5(url.encode()).hexdigest()


def url_to_cache_dir(url: str) -> Path:
    return TC_CACHE_DIR / url_to_md5(url)


def get_problem_urls() -> dict[str, list[str]]:
    """全テストファイルから PROBLEM URL を収集して重複排除。URL → サービス名のヒント"""
    urls: dict[str, list[str]] = {}
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


def try_downloaded_zip(url: str) -> bool:
    """前回 CI でダウンロードした tc-{service}.zip からコピー"""
    md5 = url_to_md5(url)
    cache_dir = url_to_cache_dir(url)
    # サービスごとの zip が展開されたディレクトリを探す
    for sub in TC_DOWNLOADED_DIR.iterdir() if TC_DOWNLOADED_DIR.exists() else []:
        candidate = sub / md5 if sub.is_dir() else None
        if candidate and candidate.exists() and any(candidate.iterdir()):
            cache_dir.mkdir(parents=True, exist_ok=True)
            subprocess.run(["cp", "-r", f"{candidate}/.", str(cache_dir)], check=True)
            return True
    # フラットな構造も試す
    flat = TC_DOWNLOADED_DIR / md5
    if flat.exists() and any(flat.iterdir()):
        cache_dir.mkdir(parents=True, exist_ok=True)
        subprocess.run(["cp", "-r", f"{flat}/.", str(cache_dir)], check=True)
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
    """yukicoder のテストケースをダウンロード"""
    cache_dir = url_to_cache_dir(url)
    if not YUKICODER_TOKEN:
        print(f"    yukicoder: no token set", flush=True)
        return False

    m = re.search(r"/problems/no/(\d+)", url)
    if not m:
        print(f"    yukicoder: cannot parse problem number from {url}", flush=True)
        return False
    problem_no = m.group(1)

    try:
        api_url = f"https://yukicoder.me/api/v1/problems/no/{problem_no}/file/in"
        req = urllib.request.Request(api_url)
        req.add_header("Authorization", f"Bearer {YUKICODER_TOKEN}")
        with urllib.request.urlopen(req, timeout=30) as resp:
            in_names = resp.read().decode().strip().split("\n")
    except urllib.error.HTTPError as e:
        print(f"    yukicoder #{problem_no}: HTTP {e.code} {e.reason}", flush=True)
        return False
    except Exception as e:
        print(f"    yukicoder #{problem_no}: {e}", flush=True)
        return False

    # 一時ディレクトリで作業
    tmp_dir = Path(str(cache_dir) + ".tmp")
    if tmp_dir.exists():
        shutil.rmtree(tmp_dir)
    tmp_dir.mkdir(parents=True, exist_ok=True)

    count = 0
    for name in in_names:
        name = name.strip()
        if not name:
            continue
        try:
            in_url = f"https://yukicoder.me/api/v1/problems/no/{problem_no}/file/in/{name}"
            req = urllib.request.Request(in_url)
            req.add_header("Authorization", f"Bearer {YUKICODER_TOKEN}")
            with urllib.request.urlopen(req, timeout=30) as resp:
                in_data = resp.read()

            out_url = f"https://yukicoder.me/api/v1/problems/no/{problem_no}/file/out/{name}"
            req = urllib.request.Request(out_url)
            req.add_header("Authorization", f"Bearer {YUKICODER_TOKEN}")
            with urllib.request.urlopen(req, timeout=30) as resp:
                out_data = resp.read()

            base = Path(name).stem
            (tmp_dir / f"{base}.in").write_bytes(in_data)
            (tmp_dir / f"{base}.out").write_bytes(out_data)
            count += 1
        except urllib.error.HTTPError as e:
            print(f"    yukicoder #{problem_no} case {name}: HTTP {e.code} {e.reason}", flush=True)
            shutil.rmtree(tmp_dir)
            return False
        except Exception as e:
            print(f"    yukicoder #{problem_no} case {name}: {e}", flush=True)
            shutil.rmtree(tmp_dir)
            return False
        time.sleep(0.1)  # レートリミット対策

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
    urls = get_problem_urls()
    print(f"Found {len(urls)} unique problem URLs")

    cached = 0
    from_zip = 0
    from_downloaded = 0
    to_download: list[str] = []

    # 1. キャッシュ済み / tc.zip / tc-downloaded.zip から取得
    for url in urls:
        if is_cached(url):
            cached += 1
            continue
        if try_tc_zip(url):
            from_zip += 1
            continue
        if try_downloaded_zip(url):
            from_downloaded += 1
            continue
        to_download.append(url)

    print(f"  Cached: {cached}, From tc.zip: {from_zip}, From tc-downloaded: {from_downloaded}")
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

    # yosupo は順次実行（リポジトリ内ファイル操作があるため）
    if yosupo_urls:
        print(f"  Generating {len(yosupo_urls)} yosupo problems...")
        for url in yosupo_urls:
            m = re.search(r"/problem/(\w+)", url)
            name = m.group(1) if m else url
            print(f"  [GEN] {name} ...", end="", flush=True)
            if download_one(url):
                downloaded += 1
                print(" OK")
            else:
                failed += 1
                print(" FAILED")

    print(f"\nTestcase download summary:")
    print(f"  Cached:          {cached}")
    print(f"  From tc.zip:     {from_zip}")
    print(f"  From downloaded: {from_downloaded}")
    print(f"  Downloaded:      {downloaded}")
    print(f"  Failed:          {failed}")
    print(f"  Total:           {len(urls)}")


if __name__ == "__main__":
    main()
