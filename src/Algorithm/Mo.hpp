#pragma once
#include <bits/stdc++.h>
/**
 * @title Mo
 * @category アルゴリズム
 *  O((N+Q)√N)
 * @see https://ei1333.hateblo.jp/entry/2017/09/11/211011
 */

// BEGIN CUT HERE

struct Mo {
  int n;
  std::vector<std::pair<int, int> > lr;
  explicit Mo(int n) : n(n) {}
  /* [l, r) */
  void query(int l, int r) { lr.emplace_back(l, r); }

  template <typename AL, typename AR, typename EL, typename ER, typename O>
  void run(const AL &add_left, const AR &add_right, const EL &erase_left,
           const ER &erase_right, const O &out) {
    int q = (int)lr.size();
    int bs = n / std::min<int>(n, std::sqrt(q));
    std::vector<int> ord(q);
    std::iota(ord.begin(), ord.end(), 0);
    std::sort(ord.begin(), ord.end(), [&](int a, int b) {
      int ablock = lr[a].first / bs, bblock = lr[b].first / bs;
      if (ablock != bblock) return ablock < bblock;
      return (ablock & 1) ? lr[a].second > lr[b].second
                          : lr[a].second < lr[b].second;
    });
    int l = 0, r = 0;
    for (auto idx : ord) {
      while (l > lr[idx].first) add_left(--l);
      while (r < lr[idx].second) add_right(r++);
      while (l < lr[idx].first) erase_left(l++);
      while (r > lr[idx].second) erase_right(--r);
      out(idx);
    }
  }
  template <typename A, typename E, typename O>
  void run(const A &add, const E &erase, const O &out) {
    run(add, add, erase, erase, out);
  }
};