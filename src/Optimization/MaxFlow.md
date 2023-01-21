---
title: 最大流
documentation_of: ./MaxFlow.hpp
---

### インターフェース
#### `MaxFlow`
- 基本のインターフェース
- EdgePtr
  - change_cap: 容量を変更, それに伴うフローのはみ出しを出力
- 双方向へ流れうる辺も可

#### `MaxFlowLowerBound` 
- 最小流量制限付きのインターフェース
- 容量は負も可 (逆向きに流れる意味)

### 内部アルゴリズム
#### `Dinic`
- 計算量 $\mathcal{O}(n^2 m)$

#### `PushRelabel`
- 計算量 $\mathcal{O}(n^2 \sqrt{m})$

## 問題例
[LibreOJ #127. 最大流 加强版](https://loj.ac/p/127) (Dinicだと落ちる)

## 参考
[https://misawa.github.io/others/flow/dinic_time_complexity.html](https://misawa.github.io/others/flow/dinic_time_complexity.html)