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
// https://atcoder.jp/contests/abc172/tasks/abc172_d
// https://atcoder.jp/contests/xmascon19/tasks/xmascon19_d

// BEGIN CUT HERE

// sum f s.t. f :=  h * g^(-1)

template <class T, class G, class H>
T dirichlet_inv_sum(unsigned long long N, const G &gsum, const H &hsum,
                    std::unordered_map<unsigned long long, T> &memo) {
  auto it = memo.find(N);
  if (it != memo.end()) return it->second;
  T ret = hsum(N);
  for (unsigned long long d = 2, nN = double(N) / d, nd; nN;
       nN = double(N) / (d = nd))
    ret -= dirichlet_inv_sum(nN, gsum, hsum, memo)
           * (gsum((nd = double(N) / nN + 1) - 1) - gsum(d - 1));
  return memo[N] = ret / gsum(1);
}
template <class T, class G, class H>
T dirichlet_inv_sum(std::uint64_t N, const G &gsum, const H &hsum) {
  std::unordered_map<unsigned long long, T> memo;
  return dirichlet_inv_sum<T>(N, gsum, hsum, memo);
}

// sum f s.t. f :=  h * g
template <class T, class G, class H>
T dirichlet_mul_sum(std::uint64_t N, const G &gsum, const H &hsum) {
  const int sqrtN = sqrt(N);
  T ret = 0;
  for (int i = 1; i <= sqrtN; i++) ret += (hsum(i) - hsum(i - 1)) * gsum(N / i);
  for (int i = 1; i <= sqrtN; i++) ret += (gsum(i) - gsum(i - 1)) * hsum(N / i);
  return ret -= hsum(sqrtN) * gsum(sqrtN);
}