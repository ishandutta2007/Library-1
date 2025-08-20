---
title: マトロイド交叉
documentation_of: ../../src/Optimization/matroid_intersection.hpp
---

## 概要
2つのマトロイド $M_1 = (S, \mathcal{I}_1), M_2 = (S, \mathcal{I}_2)$ が与えられたとき、共通独立集合 $I \in \mathcal{I}_1 \cap \mathcal{I}_2$ のうち、要素数が最大のもの（最大共通独立集合）や、要素に重みがある場合に重みが最大／最小となるものを求める問題。

このライブラリは、重みなし・重み付きのマトロイド交叉問題を解くための関数を提供します。

---

## マトロイドインターフェース
マトロイドを表すクラスは、以下のインターフェースを満たす必要があります。

```cpp
struct YourMatroid {
  // 現在の独立集合 I に基づいて内部状態を構築する
  void build(const std::vector<int>& I);

  // 要素 e が I に追加可能かを判定する
  // I は build で渡された集合
  bool oracle(int e) const;

  // I に e を追加したとき、I から f を除けば独立性を保てるかを判定する
  // (I \ {f}) U {e} が独立であるかを判定する
  bool oracle(int e, int f) const;
};
```

---

## 提供されるマトロイド

### `GraphicMatroid`
グラフ理論における森（閉路を含まない辺集合）を独立集合とするマトロイドです。

- `GraphicMatroid(int n)`: `n`頂点のグラフマトロイドを構築する。
- `int add_edge(int u, int v)`: 辺`(u, v)`を追加し、その辺のIDを返す。

### `PartitionMatroid`
台集合 $S$ を disjoint な部分集合 $S_1, S_2, \dots, S_k$ に分割し、各部分集合から高々 $R_i$ 個の要素を選ぶ集合を独立集合とするマトロイドです。

- `PartitionMatroid(int m, const std::vector<std::vector<int>>& parts, const std::vector<int>& R)`:
    - `m`: 台集合の要素数
    - `parts`: 台集合の分割。`parts[i]`は $S_i$ の要素のリスト。
    - `R`: 各部分集合から選べる要素数の上限。`R[i]`は $S_i$ から選べる上限数。

---

## 関数

### 重みなし

| 関数名                          | 概要                                                                                | 計算量                      |
| ------------------------------- | ----------------------------------------------------------------------------------- | --------------------------- |
| `matroid_intersection(n,M1,M2)` | 2つのマトロイド`M1`, `M2`の最大共通独立集合を求める。<br> `n`: 台集合のサイズ。 | $O(nr^{1.5}\tau)$ |

### 重みあり

| 関数名                                          | 概要                                                                                                                                                                                                  | 計算量                  |
| ----------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------- |
| `weighted_matroid_intersection<sgn>(n,M1,M2,w)` | 2つのマトロイド`M1`, `M2`の重み付き最大／最小共通独立集合を求める。<br> `n`: 台集合のサイズ。<br> `w`: 各要素の重み。<br> `sgn`: `MAXIMIZE`または`MINIMIZE`を指定。<br> **返り値**: `std::vector<std::vector<int>>`。`ret[k]`に、サイズ`k`の共通独立集合のうち重みが最大／最小のものが格納される。 | $O(nr^2\tau)$ |

**補足:**
- $n$は台集合のサイズ, $r$は最大共通独立集合のサイズ, $\tau$は独立性オラクル呼び出しにかかる時間。

---

## コード例
色付きの辺を持つグラフで、各色の辺の数を制限しながら全域森を求める問題を考えます。これはグラフマトロイドと分割マトロイドの交叉問題として定式化できます。

```cpp
#include <iostream>
#include "Optimization/matroid_intersection.hpp"

int main() {
    // 頂点数6、辺数8のグラフ
    int n_vertices = 6;
    int n_edges = 8;

    GraphicMatroid m1(n_vertices);
    // edges[i] = {u, v, color}
    std::vector<std::tuple<int, int, int>> edges = {
        {0, 1, 0}, {0, 2, 1}, {1, 2, 0}, {1, 3, 2},
        {2, 4, 1}, {3, 4, 0}, {3, 5, 2}, {4, 5, 1}
    };
    for (const auto& [u, v, c] : edges) {
        m1.add_edge(u, v);
    }

    // 分割マトロイド: 各色の辺は1本まで
    // part 0: color 0, part 1: color 1, part 2: color 2
    std::vector<std::vector<int>> parts(3);
    for (int i = 0; i < n_edges; ++i) {
        parts[std::get<2>(edges[i])].push_back(i);
    }
    // 各色の上限は1
    std::vector<int> R = {1, 1, 1};
    PartitionMatroid m2(n_edges, parts, R);

    // 最大共通独立集合（= 各色1本以下の辺からなる森）を求める
    std::vector<int> result = matroid_intersection(n_edges, m1, m2);

    // 結果: {0, 4, 6} (辺0, 4, 6)
    // 辺0: (0,1) color 0
    // 辺4: (2,4) color 1
    // 辺6: (3,5) color 2
    // これらは森をなし、各色1本ずつという制約を満たす。
    std::cout << "Maximum common independent set (edge IDs):" << std::endl;
    for (int i = 0; i < result.size(); ++i) {
        std::cout << result[i] << (i == result.size() - 1 ? "" : " ");
    }
    std::cout << std::endl;

    return 0;
}
```

## 参考
- [https://hitonanode.github.io/cplib-cpp/combinatorial_opt/matroid_intersection.hpp](https://hitonanode.github.io/cplib-cpp/combinatorial_opt/matroid_intersection.hpp)
- William H. Cunningham. Improved bounds for matroid partition and intersection algorithms. SIAMJournal on Computing (SICOMP), 15(4):948–957, 1986.

## Verify
- [beecrowd | 2128 Demonstration of Honesty!](https://www.beecrowd.com.br/judge/en/problems/view/2128) (グラフ+分割) 
- [Hello 2020 G. Seollal - Codeforces](https://codeforces.com/contest/1284/problem/G) (グラフ+分割)
- [2019 Petrozavodsk Winter Camp, Yandex Cup D. Pick Your Own Nim - Codeforces](https://codeforces.com/gym/102156/problem/D) (二値+分割)
- [幾何コンテスト2013 B - 玉座の間](https://atcoder.jp/contests/geocon2013/tasks/geocon2013_b) (重み二部マッチング, 重みが実数) 
- [ACM-ICPC Japan Alumni Group Summer Camp 2012 Day 2 まるかいて](https://onlinejudge.u-aizu.ac.jp/problems/2429) (重み二部マッチング, |台集合| <= 1e4, 構築, sp judge) 
- [AtCoder Library Practice Contest E - MinCostFlow](https://atcoder.jp/contests/practice2/tasks/practice2_e) (重み付き, 分割+分割) 
- [Deltix Round, Summer 2021 H. DIY Tree - Codeforces](https://codeforces.com/contest/1556/problem/H) (重み付き, グラフ+分割) 
- [NAIPC 2018 G Rainbow Graph](https://www.acmicpc.net/problem/16046)(重み付き, あるマトロイド (グラフマトロイドの双対マトロイドと自由マトロイドの直和?) 同士の交差)
