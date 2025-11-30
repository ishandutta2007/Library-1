---
title:  ネットワーク単体法 (Network Simplex)
documentation_of: ../../src/Optimization/NetworkSimplex.hpp
---

##  概要

ネットワーク単体法を用いて、最小費用b-flow問題 (Minimum Cost b-flow Problem) を解くライブラリ。

各頂点 $i$ に供給量または需要量 $b_i$ が設定され、各辺に容量制約（下限・上限）とコストが与えられたグラフで、フローの制約を満しつつ総費用を最小化（または最大化）する問題を解くことができる。

- 全ての頂点 $i \in V$ について、供給量（需要量）の総和が0である必要がある ($\sum_{i \in V} b_i = 0$)。
- 辺 $(u, v)$ の流量 $f(u, v)$ は `low` $\le f(u, v) \le$ `up` を満たす。
- 各頂点 $i$ で、流れ出すフローの総量と流れ込むフローの総量の差が $b_i$ に等しい（フロー保存則）。
- 総費用 $\sum_{(u,v) \in E} f(u, v) \times \text{cost}(u, v)$ を最小化（または最大化）する。

最小費用流、最大流、割り当て問題など、多くのフロー問題の一般化となっている。負コストの辺、自己ループ、多重辺にも対応している。

##  テンプレートパラメータ

```cpp
template <typename flow_t, typename cost_t, MinMaxEnum obj = MINIMIZE>
class NetworkSimplex;
```

- `flow_t`: 流量を表す型 (例: `long long`)。
- `cost_t`: コストを表す型 (例: `long long`)。コストが実数の場合にも対応。
- `obj`: `MINIMIZE` (最小化、デフォルト) または `MAXIMIZE` (最大化) を指定。

## API

### `NetworkSimplex`

#### コンストラクタ

`NetworkSimplex(int n = 0)`
頂点数 `n` のグラフを構築する。

#### 頂点の追加

`int add_vertex()`
頂点を1つ追加する。新しい頂点の番号を返す。

`std::vector<int> add_vertices(int size)`
`size`個の頂点を追加する。新しい頂点たちの番号のリストを返す。

#### 辺の追加

`EdgePtr add_edge(int u, int v, flow_t low, flow_t up, cost_t cost)`
頂点 `u` から `v` へ、流量の下限 `low`、上限 `up`、単位流量あたりのコスト `cost` の辺を追加する。
`obj` が `MAXIMIZE` の場合、内部でコストの符号が反転される。戻り値の `EdgePtr` を通じて、実行後に辺の情報を取得できる。

#### 供給/需要の設定

`void add_supply(int u, flow_t supply)`
頂点 `u` の供給量を `supply` に設定する ($b_u > 0$ の場合)。

`void add_demand(int u, flow_t demand)`
頂点 `u` の需要量を `demand` に設定する ($b_u < 0$ の場合)。内部的には `add_supply(u, -demand)` として扱われる。

`flow_t get_supply(int u)`
頂点 `u` の供給量を取得する。

#### 実行

`bool b_flow()`
最小（または最大）費用b-flowを計算する。フローが実行可能な場合は `true` を、不可能な（制約を満たすフローが存在しない）場合は `false` を返す。
総供給量と総需要量が一致しない場合 (`sum(b_i) != 0`) も実行不可能と判定される。

#### 結果の取得

`auto get_result_value<cost_sum_t = cost_t>()`
目的関数（総費用）の値を返す。総費用が `cost_t` の範囲を超える可能性がある場合は、`__int128_t` のようなより大きな型 `cost_sum_t` を明示的に指定する。

`cost_t get_potential(int u)`
頂点 `u` のポテンシャル $p_u$ を取得する。これは双対問題の変数に対応し、ソースコードの実装では、任意の辺 $(u, v)$ について被約コストが $c'_{uv} = c_{uv} + p_u - p_v$ となるように定義されている。

### `EdgePtr`

`add_edge` の戻り値。辺の状態を取得するためのポインタライクなオブジェクト。

- `int src()`: 辺の始点を返す。
- `int dst()`: 辺の終点を返す。
- `flow_t flow()`: 辺の流量を返す。
- `flow_t lower()`: 辺の流量の下限を返す。
- `flow_t upper()`: 辺の流量の上限を返す。
- `cost_t cost()`: 辺のコストを返す (`obj` の影響を受けない元のコスト)。
- `cost_t gain()`: 辺のコストの符号を反転した値 (`-cost`) を返す。最大化問題で利益と見なす場合に便利。

## 計算量

理論的な最悪計算時間は指数時間だが、多くの問題に対して実用上は高速に動作する。

## 使用例

[Minimum Cost b-flow (Yosupo Judge)](https://judge.yosupo.jp/problem/min_cost_b_flow)

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "src/Optimization/NetworkSimplex.hpp"

// __int128_t の出力設定
namespace std {
std::ostream& operator<<(std::ostream& os, __int128_t value) {
    if (value == 0) return os << "0";
    std::string s;
    bool neg = value < 0;
    if (neg) value = -value;
    while (value > 0) {
        s += (char)(value % 10 + '0');
        value /= 10;
    }
    if (neg) s += '-';
    std::reverse(s.begin(), s.end());
    return os << s;
}
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int N, M;
    std::cin >> N >> M;

    using MCF = NetworkSimplex<long long, long long>;
    MCF graph(N);
    std::vector<MCF::EdgePtr> edges;

    // 各頂点の供給/需要 (b_i) を設定
    for (int i = 0; i < N; i++) {
        long long b;
        std::cin >> b;
        graph.add_supply(i, b);
    }

    // 辺を追加
    for (int i = 0; i < M; i++) {
        long long s, t, l, u, c;
        std::cin >> s >> t >> l >> u >> c;
        edges.emplace_back(graph.add_edge(s, t, l, u, c));
    }

    if (graph.b_flow()) {
        // 最小費用
        std::cout << graph.get_result_value<__int128_t>() << '\n';
        // 各頂点のポテンシャル
        for (int i = 0; i < N; i++) {
            std::cout << graph.get_potential(i) << '\n';
        }
        // 各辺の流量
        for (auto &e : edges) {
            std::cout << e.flow() << '\n';
        }
    } else {
        std::cout << "infeasible" << '\n';
    }

    return 0;
}
```

## Verify

- [幾何コンテスト2013 B - 玉座の間](https://atcoder.jp/contests/geocon2013/tasks/geocon2013_b) (コスト実数)
- [Minimum Cost b-flow (Yosupo Judge)](https://judge.yosupo.jp/problem/min_cost_b_flow)
- [Assignment Problem (Yosupo Judge)](https://judge.yosupo.jp/problem/assignment)
- [GRL_6_B - Minimum Cost Flow (AOJ)](https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_B)
