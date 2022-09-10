#pragma once
#include <bits/stdc++.h>
/**
 * @title 杜教筛
 * @category 数学
 * @see https://maspypy.com/yukicoder-no-886-direct
 * @see https://yukicoder.me/problems/no/1019/editorial
 * @see https://en.wikipedia.org/wiki/M%C3%B6bius_inversion_formula
 * @see https://yukicoder.me/wiki/sum_totient
 * @see https://oi-wiki.org/math/du/
 * dirichlet_inv_sum : O(N^(3/4))
 *  (ただし前処理でN^(2/3)まで計算できるならO(N^(2/3)))
 * dirichlet_mul_sum : O(√N)
 */

// verify用:
// https://atcoder.jp/contests/xmascon19/tasks/xmascon19_d

// BEGIN CUT HERE

// sum f s.t. f :=  h * g^(-1)
template <class T, class G, class H>
T dirichlet_div_sum(std::uint64_t N, const G &gsum, const H &hsum,
                    std::unordered_map<std::uint64_t, T> &memo) {
  if (auto it = memo.find(N); it != memo.end()) return it->second;
  T ret = hsum(N), gs1 = gsum(1), gs = gs1, ngs;
  for (std::uint64_t d = 1, nN; nN = N / (d + 1); gs = ngs)
    ret -= dirichlet_div_sum(nN, gsum, hsum, memo) *
           ((ngs = gsum(d = N / nN)) - gs);
  return memo[N] = ret / gs1;
}
template <class T, class G, class H>
T dirichlet_div_sum(std::uint64_t N, const G &gsum, const H &hsum) {
  std::unordered_map<std::uint64_t, T> memo;
  return dirichlet_div_sum<T>(N, gsum, hsum, memo);
}

// sum f s.t. f :=  h * g
template <class T, class G, class H>
T dirichlet_mul_sum(std::uint64_t N, const G &gsum, const H &hsum) {
  const int sqrtN = std::sqrt(N);
  T ret = 0;
  for (int i = 1; i <= sqrtN; i++) ret += (hsum(i) - hsum(i - 1)) * gsum(N / i);
  for (int i = 1; i <= sqrtN; i++) ret += (gsum(i) - gsum(i - 1)) * hsum(N / i);
  return ret -= hsum(sqrtN) * gsum(sqrtN);
}