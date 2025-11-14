---
title: 原始根と位数 $\mathbb{F}_p^{\times}$
documentation_of: ../../src/NumberTheory/OrderFp.hpp
---


## `OrderFp` クラス

`constexpr` できる. \
原始根を求める関数は同じ $p$ に対して繰り返し使うものでもないので二分木実装せず簡単にした.

| メンバ変数 | 概要 |
| ---| --- |
|`factors`| $p-1$ の[素因数分解](md/NumberTheory/Factors.md)|

| メンバ関数 | 概要 | 計算量  |
| ---| --- | ---|
| `OrderFp(p)` | コンストラクタ. <br> 引数の $p$ が素数でないと `assert` で落ちる. | $O \left(p^{1/4}\right) $  |
| `primitive_root()` | 素数 $p$ の原始根の一つを返す.| $O \left((\log p)^2 \right) $|
| `operator()(x)` | $x$ の位数 $\mathrm{ord}_p(x)$ を返す. <br> $x\equiv 0 \ (\mathrm{mod} \ p)$ なら $0$ を返す. |  $O \left( \log p \log \log p\right) $ |

## 参考
[https://twitter.com/_su1sen/status/1576146280795242497](https://twitter.com/_su1sen/status/1576146280795242497)