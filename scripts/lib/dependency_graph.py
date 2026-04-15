"""mylib/ 内の hpp 間の依存グラフ構築とテストマッピング (dependency-graph.ts の Python 移植)"""

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
SRC_DIR = ROOT / "mylib"
TEST_DIR = ROOT / "test"

_INCLUDE_RE = re.compile(r'#include\s+"(mylib/[^"]+\.hpp)"')


def build_dependency_graph() -> dict:
    """依存グラフを構築して返す"""
    depends_on: dict[str, list[str]] = {}
    required_by: dict[str, list[str]] = {}

    if not SRC_DIR.exists():
        return {
            "depends_on": depends_on,
            "required_by": required_by,
            "transitive_deps": {},
            "transitive_required_by": {},
        }

    for hpp_file in SRC_DIR.rglob("*.hpp"):
        content = hpp_file.read_text()
        rel = str(hpp_file.relative_to(ROOT))
        deps = _INCLUDE_RE.findall(content)
        depends_on[rel] = sorted(deps)
        for dep in deps:
            required_by.setdefault(dep, []).append(rel)

    for key in required_by:
        required_by[key].sort()

    # 推移的依存
    transitive_deps: dict[str, set[str]] = {}

    def get_transitive(hpp: str, visited: set[str]) -> set[str]:
        if hpp in transitive_deps:
            return transitive_deps[hpp]
        if hpp in visited:
            return set()
        visited.add(hpp)
        result: set[str] = set()
        for dep in depends_on.get(hpp, []):
            result.add(dep)
            result.update(get_transitive(dep, visited))
        transitive_deps[hpp] = result
        return result

    for hpp in depends_on:
        get_transitive(hpp, set())

    # 推移的逆依存
    transitive_required_by: dict[str, set[str]] = {}

    def get_transitive_req_by(hpp: str, visited: set[str]) -> set[str]:
        if hpp in transitive_required_by:
            return transitive_required_by[hpp]
        if hpp in visited:
            return set()
        visited.add(hpp)
        result: set[str] = set()
        for req in required_by.get(hpp, []):
            result.add(req)
            result.update(get_transitive_req_by(req, visited))
        transitive_required_by[hpp] = result
        return result

    all_hpps = set(depends_on.keys()) | set(required_by.keys())
    for hpp in all_hpps:
        get_transitive_req_by(hpp, set())

    return {
        "depends_on": depends_on,
        "required_by": required_by,
        "transitive_deps": transitive_deps,
        "transitive_required_by": transitive_required_by,
    }


def build_test_map(graph: dict) -> dict[str, list[str]]:
    """hpp → テストファイルのマッピングを構築"""
    mapping: dict[str, set[str]] = {}

    def add_test(hpp: str, test: str) -> None:
        mapping.setdefault(hpp, set()).add(test)

    if not TEST_DIR.exists():
        return {}

    for test_file in TEST_DIR.rglob("*.test.cpp"):
        content = test_file.read_text()
        rel = str(test_file.relative_to(ROOT))
        for m in _INCLUDE_RE.finditer(content):
            hpp = m.group(1)
            add_test(hpp, rel)
            trans = graph.get("transitive_deps", {}).get(hpp)
            if trans:
                for dep in trans:
                    add_test(dep, rel)

    return {k: sorted(v) for k, v in mapping.items()}
