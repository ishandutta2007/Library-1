"""テストケースダウンロード共通モジュール

Library / judge 両リポジトリで共有する。
各 DL 関数はキャッシュディレクトリを引数で受け取り、
呼び出し元 (Library / judge) がパスを決定する。
stdlib のみ使用。
"""

import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import urllib.error
import urllib.request
from concurrent.futures import ProcessPoolExecutor, ThreadPoolExecutor, as_completed
from dataclasses import dataclass, field
from pathlib import Path


def url_to_md5(url: str) -> str:
    return hashlib.md5(url.encode()).hexdigest()


def copy_checker_assets(
    source_dir: Path,
    dest_dir: Path,
    *,
    extra_headers: list[Path] | None = None,
) -> None:
    """checker と依存ヘッダをコピー"""
    checker_cpp = source_dir / "checker.cpp"
    if not checker_cpp.exists():
        return

    shutil.copy2(checker_cpp, dest_dir / "checker.cpp")

    for pattern in ("*.h", "*.hpp"):
        for header in source_dir.glob(pattern):
            shutil.copy2(header, dest_dir / header.name)

    for header in extra_headers or []:
        if header.exists():
            shutil.copy2(header, dest_dir / header.name)


def _atomic_replace(tmp_dir: Path, cache_dir: Path, count: int) -> bool:
    """tmp_dir に count 件あれば cache_dir にリネーム、なければ tmp_dir を削除"""
    if count > 0:
        if cache_dir.exists():
            shutil.rmtree(cache_dir)
        tmp_dir.rename(cache_dir)
        return True
    else:
        shutil.rmtree(tmp_dir)
        return False


# ============================================================
# AOJ
# ============================================================


def download_aoj(url: str, cache_dir: Path) -> bool:
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

    tmp_dir = Path(str(cache_dir) + ".tmp")
    if tmp_dir.exists():
        shutil.rmtree(tmp_dir)
    tmp_dir.mkdir(parents=True, exist_ok=True)

    expected = [
        tc for tc in headers.get("headers", []) if tc.get("serial") is not None
    ]
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
            shutil.rmtree(tmp_dir)
            return False

    return _atomic_replace(tmp_dir, cache_dir, count)


# ============================================================
# yosupo judge (Library Checker)
# ============================================================


def ensure_library_checker_repo(library_checker_dir: Path) -> bool:
    if library_checker_dir.exists():
        print("  Updating library-checker-problems...", flush=True)
        try:
            subprocess.run(
                ["git", "fetch", "--depth=1", "origin", "master"],
                check=True,
                capture_output=True,
                timeout=60,
                cwd=library_checker_dir,
            )
            subprocess.run(
                ["git", "reset", "--hard", "origin/master"],
                check=True,
                capture_output=True,
                timeout=30,
                cwd=library_checker_dir,
            )
            return True
        except Exception as e:
            print(f"  Update failed: {e}", flush=True)
            return True
    else:
        print("  Cloning library-checker-problems...", flush=True)
        try:
            subprocess.run(
                [
                    "git",
                    "clone",
                    "--depth=1",
                    "https://github.com/yosupo06/library-checker-problems.git",
                    str(library_checker_dir),
                ],
                check=True,
                capture_output=True,
                timeout=120,
            )
        except Exception as e:
            print(f"  Clone failed: {e}", flush=True)
            return False

    subprocess.run(
        [sys.executable, "-m", "pip", "install", "-q", "pyyaml", "toml"],
        capture_output=True,
    )
    return True


def download_yosupo(
    url: str, cache_dir: Path, library_checker_dir: Path
) -> bool:
    m = re.search(r"/problem/(\w+)", url)
    if not m:
        return False
    problem_id = m.group(1)

    if not ensure_library_checker_repo(library_checker_dir):
        return False

    problem_dir = None
    for p in library_checker_dir.rglob("info.toml"):
        if p.parent.name == problem_id:
            problem_dir = p.parent
            break
    if not problem_dir:
        return False

    try:
        subprocess.run(
            [
                "bash",
                "-c",
                f"ulimit -s unlimited && {sys.executable} "
                f"{library_checker_dir / 'generate.py'} "
                f"{problem_dir / 'info.toml'}",
            ],
            check=True,
            timeout=300,
            cwd=library_checker_dir,
        )
    except Exception as e:
        print(f"    yosupo generate failed for {problem_id}: {e}", flush=True)
        return False

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

    copy_checker_assets(
        problem_dir,
        tmp_dir,
        extra_headers=[library_checker_dir / "common" / "testlib.h"],
    )

    return _atomic_replace(tmp_dir, cache_dir, count)


# ============================================================
# yukicoder
# ============================================================


def download_yukicoder(url: str, cache_dir: Path, token: str) -> bool:
    import io
    import zipfile

    if not token:
        print("    yukicoder: no token set", flush=True)
        return False

    m = re.search(r"/problems/no/(\d+)", url)
    if not m:
        print(
            f"    yukicoder: cannot parse problem number from {url}",
            flush=True,
        )
        return False
    problem_no = m.group(1)

    zip_url = f"https://yukicoder.me/problems/no/{problem_no}/testcase.zip"
    req = urllib.request.Request(zip_url)
    req.add_header("Authorization", f"Bearer {token}")

    try:
        with urllib.request.urlopen(req, timeout=60) as resp:
            zip_data = resp.read()
    except urllib.error.HTTPError as e:
        print(
            f"    yukicoder #{problem_no}: HTTP {e.code} {e.reason}",
            flush=True,
        )
        return False
    except Exception as e:
        print(f"    yukicoder #{problem_no}: {e}", flush=True)
        return False

    tmp_dir = Path(str(cache_dir) + ".tmp")
    if tmp_dir.exists():
        shutil.rmtree(tmp_dir)
    tmp_dir.mkdir(parents=True, exist_ok=True)

    try:
        with zipfile.ZipFile(io.BytesIO(zip_data)) as zf:
            inputs: dict[str, bytes] = {}
            outputs: dict[str, bytes] = {}
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
                    (tmp_dir / f"{case_name}.out").write_bytes(
                        outputs[case_name]
                    )
                    count += 1
    except Exception as e:
        print(
            f"    yukicoder #{problem_no}: zip extract failed: {e}",
            flush=True,
        )
        shutil.rmtree(tmp_dir)
        return False

    return _atomic_replace(tmp_dir, cache_dir, count)


# ============================================================
# HackerRank
# ============================================================


def download_hackerrank(url: str, cache_dir: Path) -> bool:
    import io
    import zipfile

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
        print(
            f"    HackerRank {challenge}: HTTP {e.code} {e.reason}",
            flush=True,
        )
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
                if name.startswith("input/") and name.endswith(".txt"):
                    case_name = Path(name).stem
                    in_data = zf.read(name)
                    out_name = name.replace("input/", "output/").replace(
                        "input", "output"
                    )
                    if out_name in zf.namelist():
                        out_data = zf.read(out_name)
                        (tmp_dir / f"{case_name}.in").write_bytes(in_data)
                        (tmp_dir / f"{case_name}.out").write_bytes(out_data)
                        count += 1
    except Exception:
        shutil.rmtree(tmp_dir)
        return False

    return _atomic_replace(tmp_dir, cache_dir, count)


# ============================================================
# ディスパッチ
# ============================================================


@dataclass
class DownloadConfig:
    """ダウンロード設定 (呼び出し元リポジトリが設定する)"""

    tc_cache_dir: Path
    library_checker_dir: Path
    yukicoder_token: str = ""


def download_one(url: str, config: DownloadConfig) -> bool:
    """URL に応じたダウンロード関数を呼ぶ"""
    cache_dir = config.tc_cache_dir / url_to_md5(url)
    if "onlinejudge.u-aizu.ac.jp" in url:
        return download_aoj(url, cache_dir)
    elif "judge.yosupo.jp" in url:
        return download_yosupo(url, cache_dir, config.library_checker_dir)
    elif "yukicoder.me" in url:
        return download_yukicoder(url, cache_dir, config.yukicoder_token)
    elif "hackerrank.com" in url:
        return download_hackerrank(url, cache_dir)
    else:
        return False


def _download_one_safe(
    url: str, config: DownloadConfig
) -> tuple[str, bool]:
    try:
        return (url, download_one(url, config))
    except Exception as e:
        print(f"    Error: {url}: {e}", file=sys.stderr)
        return (url, False)


def is_cached(url: str, config: DownloadConfig) -> bool:
    cache_dir = config.tc_cache_dir / url_to_md5(url)
    return cache_dir.exists() and any(cache_dir.iterdir())


@dataclass
class DownloadSummary:
    cached: int = 0
    downloaded: int = 0
    failed: int = 0
    total: int = 0
    extra: dict[str, int] = field(default_factory=dict)


def download_batch(
    urls: dict[str, list[str]],
    config: DownloadConfig,
) -> DownloadSummary:
    """複数 URL を一括ダウンロード。yosupo は ProcessPool、他は ThreadPool で並列化。"""
    summary = DownloadSummary(total=len(urls))

    to_download: list[str] = []
    for url in urls:
        if is_cached(url, config):
            summary.cached += 1
        else:
            to_download.append(url)

    print(f"  Cached: {summary.cached}")
    print(f"  Need download: {len(to_download)}")

    if not to_download:
        print("All testcases available!")
        return summary

    yosupo_urls = [u for u in to_download if "judge.yosupo.jp" in u]
    other_urls = [u for u in to_download if "judge.yosupo.jp" not in u]

    if other_urls:
        print(f"  Downloading {len(other_urls)} problems (parallel)...")
        with ThreadPoolExecutor(max_workers=8) as executor:
            futures = {
                executor.submit(_download_one_safe, url, config): url
                for url in other_urls
            }
            for future in as_completed(futures):
                url, success = future.result()
                if success:
                    summary.downloaded += 1
                    print(f"  [OK]  {url}")
                else:
                    summary.failed += 1
                    print(f"  [NG]  {url}")

    if yosupo_urls:
        if not ensure_library_checker_repo(config.library_checker_dir):
            print(
                "  Failed to prepare library-checker-problems, skipping yosupo"
            )
            summary.failed += len(yosupo_urls)
        else:
            print(
                f"  Generating {len(yosupo_urls)} yosupo problems (parallel)..."
            )
            with ProcessPoolExecutor(max_workers=1) as executor:
                futures = {
                    executor.submit(_download_one_safe, url, config): url
                    for url in yosupo_urls
                }
                for future in as_completed(futures):
                    url, success = future.result()
                    m = re.search(r"/problem/(\w+)", url)
                    name = m.group(1) if m else url
                    if success:
                        summary.downloaded += 1
                        print(f"  [GEN] {name} ... OK", flush=True)
                    else:
                        summary.failed += 1
                        print(f"  [GEN] {name} ... FAILED", flush=True)

    return summary
