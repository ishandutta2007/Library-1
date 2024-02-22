---
title: 周期性の利用 (Functionalグラフ)
documentation_of: ../../src/Misc/Period.hpp
---

## `Period<T>` クラス

初期状態が $x_0\in T$ な列 $(x_i)_i$ が次の漸化式で定まっているとする．

$
\displaystyle x_{i+1} = f(x_i)
$

$x_i$ として取りうる状態の数が有限であるときに周期性を利用して巨大な数 $k$ に対する $x_k$ を求める　

などするクラス．

初期状態が複数ある場合でも Functional グラフを重軽分解することで頑張る．

状態 $x$ をいい感じに並び替えて，ラベリングする．

状態の数を $n$ とする．

| メンバ関数 | 概要 | 計算量 |
| --- | --- | --- |
| `Period(f, init)` | コンストラクタ. <br> 関数 $f:T\to T$ と初期状態の集合 $X_0 \subset T $ を渡す． <br> 第二引数は `vector<T>`. |$O(An\log n)$ <br> ただし $f(x)$ の計算に $O(A)$ とかかるとした．<br> $\log$ は連想配列を用いたため．|
| `Period(functional)` | コンストラクタ. <br> 全状態の集合が $V＝\lbrace 0,\dots,n-1\rbrace$ だとして，それぞれの状態の移り先を表す配列 ( $V\to V$ ) を渡す．<br> 引数は `vector<int>`. <br> `T == int` でないと呼べない．| $O(n)$|
|`size()`|状態の数 $n$ を返す．||
|`operator()(x)`| $x$ に対応するラベルを返す．<br> 戻り値は `int`. |
| `jump(x,k)`          | $f^k(x)=\overbrace{f(\cdots f(f}^{k}(x)))$ を返す. <br> 第二引数は何らかの整数型 `Int`．( [`BigInt`クラス](../FFT/BigInt.hpp)も使える．) <br> 戻り値は `T`.| $O(\log n)$ |
| `path(x,k)`          | $x,f(x),\dots,f^k(x)$ という軌道を表す 半開区間の列 を返す．<br> 第二引数は何らかの整数型 `Int`．( [`BigInt`クラス](../FFT/BigInt.hpp)も使える．) <br> 戻り値は4つのデータをラッピングした`tuple`．<br> 一つ目はサイクルに至るまでの軌道．<br> 二つ目はサイクル一周分を表す軌道．<br> 三つ目はサイクルの周回数 `Int`. <br> 四つ目はサイクルの余りの軌道．<br> 軌道は `vector<pair<int,int>>` で表現する半開区間の列 $\lbrack a_0, b_0 ),\dots,\lbrack a_m, b_m )$ であり, `operator()` のラベルに対応している． | $O(\log n)$ |
| `path_upto_cycle(x)`          | 同じ値が出るまでの軌道を返す．<br> 軌道は `vector<pair<int,int>>` で表現する半開区間の列 $\lbrack a_0, b_0 ),\dots,\lbrack a_m, b_m )$ であり, `operator()` のラベルに対応している． | $O(\log n)$ |

## 問題例
[AtCoder Beginner Contest 030 D - へんてこ辞書](https://atcoder.jp/contests/abc030/tasks/abc030_d)\
[JOI 2010-2011 日本代表選手選考会 (春合宿代替措置) 過去問 report - 報告 (Report)](https://atcoder.jp/contests/joisc2011/tasks/joisc2011_report) (`path_upto_cycle`)