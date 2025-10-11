---
title: 区間をsetで管理するやつ
documentation_of: ../../src/DataStructure/RangeSet.hpp
---

## 概要
`std::set` を用いて、互いに素な閉区間の集合を管理するデータ構造。
区間の追加、削除、検索などを対数時間で行うことができる。

## テンプレートパラメータ
- `Int`: 区間の端点を表す整数型。
- `bool merge = true`:
  - `true` の場合: `[a, b]` と `[b+1, c]` のように隣接する区間が生成された際に、自動的に `[a, c]` にマージする。
  - `false` の場合: 隣接する区間をマージしない。

---

## `ClosedSection` 構造体
閉区間 $[l, r]$ を表す構造体。

### メンバ変数
- `l`, `r`: それぞれ区間の左端と右端を表す。

### メンバ関数
- `length()`: 区間の長さ ($r - l + 1$) を返す。
- `operator bool()`: 区間が有効なとき (`l <= r`) に `true` を返す。

---

## メンバ関数

| 関数名 | 概要 | 計算量 |
|:---|:---|:---|
| `RangeSet()` | コンストラクタ。 | $O(1)$ |
| `covered_by(l, r)` <br> `covered_by(x)` <br> `covered_by(cs)` | 指定した区間または点を完全に包含する区間を返す。存在しない場合は `l > r` となる無効な区間を返す。 | $O(\log N)$ |
| `insert(l, r)` <br> `insert(x)` <br> `insert(cs)` | 区間を追加する。既存の区間と重なる部分はマージされる。返り値は新たに追加された要素の総数。 | $O(k \log N)$ |
| `erase(l, r)` <br> `erase(x)` <br> `erase(cs)` | 指定した区間と重なる部分を削除する。返り値は削除された要素の総数。 | $O(k \log N)$ |
| `size()` | 管理している区間の数を返す（番兵は除く）。 | $O(1)$ |
| `begin()`, `end()` | 管理している区間を走査するためのイテレータを返す（番兵は除く）。 | $O(1)$ |
| `mex(x)` | 管理している区間に含まれない `x` 以上の最小の整数を返す。 | $O(\log N)$ |

- $N$: 管理している区間の数。
- $k$: `insert`, `erase` で操作対象の区間と重なる、または隣接する既存の区間の数。

## その他
`std::ostream` に対応しており、`std::cout << rs;` のようにして内容を出力できる。

## Verify
- [HHKB プログラミングコンテスト 2020 C - Neq Min](https://atcoder.jp/contests/hhkb2020/tasks/hhkb2020_c)
- [AtCoder Regular Contest 039 C - 幼稚園児高橋君](https://atcoder.jp/contests/arc039/tasks/arc039_c)
- [AtCoder Beginner Contest 256 D - Union of Interval](https://atcoder.jp/contests/abc256/tasks/abc256_d)