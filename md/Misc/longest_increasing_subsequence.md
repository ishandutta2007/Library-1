---
title: 最長増加部分列
documentation_of: ../../src/Misc/longest_increasing_subsequence.hpp
---

| 関数| 概要| 計算量 |
| --- |---| ---|
|`longest_increasing_subsequence(a, strict=true)`| $\mathrm{idx}_i:=$ 添字 $i$ が LIS で使われる場合の LIS 上での位置(使われないなら $-1$ ) <br> $\mathrm{cand}_j:=$ $\mathrm{idx}_i=j$ を満たす $i$ を昇順に並べた列 <br> の二つを返す． <br> 第二引数をfalseにすると広義単調増加数列を対象にする． <br> 返り値は `pair<vector<int>, veector<vector<int>>>`| $O(n\log n)$|


## 参考
[https://miscalc.hatenablog.com/entry/2024/07/25/212618](https://miscalc.hatenablog.com/entry/2024/07/25/212618)