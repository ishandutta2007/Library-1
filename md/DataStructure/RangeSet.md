---
title: 区間をsetで管理するやつ
documentation_of: ../../src/DataStructure/RangeSet.hpp
---

## `ClosedSection` クラス
`pair<Int,Int>` みたいなもん. 閉区間  $\lbrack l, r\rbrack$ を表すクラス. \
メンバ変数 `l`, `r` を持っていてそれぞれ区間の左端，右端を表す. \
bool にキャストできて、valid (具体的には $l\le r$) ならtrueを返す. \
メンバ関数 `length()` で 閉区間の長さを返す.

## メンバ関数

| 関数名                                                                            | 概要                                                                                                                                                                                    |
| --------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `RangeSet<Int,bool>()`                                                            | コンストラクタ. 内部で 番兵 [-inf, -inf], [inf, inf]を insertしてる. 第2テンプレート引数が true なら " [a,b], [b+1,c] を [a,c] にマージ " を行う. false ならしない. (デフォルトは true) |
| 1. `covered_by(l,r)`<br> 2. `covered_by(x)` <br>3. `covered_by(ClosedSection cs)` | (1. 閉区間 [l,r], 2. 点 x, 3. ClosedSection クラス )を包含する閉区間を返す. **存在しないなら invalid な閉区間 (具体的には [1, 0]) を返す.**                                             |
| 1. `insert(l,r)` <br> 2. `insert(x)` <br> 3. `insert(ClosedSection cs)`           | (1. 閉区間 [l,r], 2. 閉区間 [x,x], 3. ClosedSection クラス )を追加. 重なっているとこは合併           .                                                                                  |
| 1. `erase(l,r)` <br> 2. `erase(x)` <br> 3. `erase(ClosedSection cs)`              | (1. 閉区間 [l,r], 2. 閉区間 [x,x], 3. ClosedSection クラス ) 部分を削除.                                                                                                                |
| `size()`                                                                          | 管理してる閉区間の数を返す. (番兵は含まない)                                                                                                                                            |
| `begin()`                                                                         | 番兵は含まないようにしてる                                                                                                                                                              |
| `end()`                                                                           | 番兵は含まないようにしてる                                                                                                                                                              |
| `mex(x)`                                                                          | 管理してる閉区間に属さない x 以上の 最小の整数を返す                                                                                                                                    |


## 問題例
[HHKB プログラミングコンテスト 2020 C - Neq Min](https://atcoder.jp/contests/hhkb2020/tasks/hhkb2020_c) \
[AtCoder Regular Contest 039 C - 幼稚園児高橋君](https://atcoder.jp/contests/arc039/tasks/arc039_c) \
[HackerRank Drawing Rectangles](https://www.hackerrank.com/contests/university-codesprint-4/challenges/drawing-rectangles)