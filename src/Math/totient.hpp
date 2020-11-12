#pragma once
#include <bits/stdc++.h>
/**
 * @title トーシェント関数
 * @category 数学
 *  O(√N)
 * ついでにテトレーション
 */

// BEGIN CUT HERE

long long totient(long long n) {
  long long ret = n;
  for (int p = 2; p * p <= n; p++)
    if (n % p == 0) {
      ret -= ret / p;
      while (n % p == 0) n /= p;
    }
  if (n > 1) ret -= ret / n;
  return ret;
}

// a↑↑b = a^(a^(a^...))
long long tetration(long long a, long long b, long long mod) {
  std::function<long long(long long, long long, long long)> rec
      = [&](long long c, long long d, long long m) {
          auto MOD = [&](long long x) { return x < m ? x : x % m + m; };
          if (c == 0) return MOD((d + 1) & 1);
          if (d == 0 || m == 1) return MOD(1);
          long long ret = MOD(1);
          for (auto e = rec(c, d - 1, totient(m)); e; e >>= 1, c = MOD(c * c))
            if (e & 1) ret = MOD(ret * c);
          return ret;
        };
  return rec(a, b, mod) % mod;
}