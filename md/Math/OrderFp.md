---
title: 原始根と位数 $\mathbb{F}_p^{\times}$
documentation_of: ../../src/Math/OrderFp.hpp
---

`constexpr` できる.

## メンバ関数

| 名前 | 概要 | 計算量  |
| ---| --- | ---|
| `OrderFp(p)` | コンストラクタ. <br> 引数の $p$ が素数でないと `assert` で落ちる. | $\mathcal{O} \left(p^{1/4}\right) $  |
| `primitive_root()` | 素数 $p$ の原始根の一つを返す.| $\mathcal{O} \left((\log p)^2 \right) $ |
| `operator()(x)` | $x$ の位数 $\mathrm{ord}_p(x)$ を返す. <br> $x\equiv 0 \ (\mathrm{mod} \ p)$ なら $0$ を返す. |  $\mathcal{O} \left( \log p \log \log p\right) $ |

