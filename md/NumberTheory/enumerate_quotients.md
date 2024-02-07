---
title: $\lfloor N/i \rfloor$ の列挙
documentation_of: ../../src/NumberTheory/enumerate_quotients.hpp
---

$\lbrack N\rbrack = \lbrace1,2,\dots,N\rbrace$ を $\left\lfloor \frac{N}{i} \right\rfloor$ が等しくなるような
$i \in \lbrack N\rbrack$ でグループ分けをする.

返り値は $\lbrace(q_j,l_j,r_j): j=1,\dots,k\rbrace$ で表現しており, 各 $j$ について任意の $i\in ( l_j,r_j\rbrack$ で $\left\lfloor \frac{N}{i} \right\rfloor = q_j$ が成り立つ.
$q_j$ が昇順になるように並べている.

|関数|概要|計算量|
|---|---|---|
|`enumerate_quotients(N)`|上の通り|$O (\sqrt{N})$|


