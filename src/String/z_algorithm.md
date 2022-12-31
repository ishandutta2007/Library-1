---
title: 最長共通接頭辞(Z-Algorithm)
documentation_of: ./z_algorithm.hpp
---
`prefix[i]` は `S` と `S.substr(i)` の LCP(longest common prefix)
## 計算量
$\mathcal{O}(|S|)$