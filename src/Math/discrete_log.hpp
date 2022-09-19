#pragma once
#include <bits/stdc++.h>
/**
 * @title 離散対数 $\mathbb{Z}/m\mathbb{Z}$
 * @category 数学
 * O(√mod)
 */

// BEGIN CUT HERE

int discrete_log(unsigned a, unsigned b, unsigned mod) {
  if (a == 0) return b == 0 ? (mod == 1 ? 0 : 1) : (b == 1 ? 0 : -1);
  auto mod_inv = [](unsigned x, unsigned mod) {
    int a = x, b = mod, u = 1, v = 0, t;
    while (b) t = a / b, std::swap(a -= t * b, b), std::swap(u -= t * v, v);
    return u >= 0 ? u % mod : (mod - (-u) % mod);
  };
  unsigned cnt = 0;
  for (unsigned g;; cnt++) {
    if ((b == 1) || (mod == 1)) return cnt;
    if ((g = std::gcd(a, mod)) == 1) break;
    if (b % g != 0) return -1;  // no solution
    b /= g, mod /= g;
    b = 1ull * mod_inv(a / g, mod) * b % mod;
  }
  unsigned middle = std::ceil(std::sqrt(mod));
  std::unordered_map<unsigned int, unsigned int> mp;
  unsigned int baby = 1;
  for (unsigned i = 0; i < middle; i++, baby = 1ull * baby * a % mod)
    mp.insert({baby, i});
  unsigned int inv = mod_inv(baby, mod);
  unsigned int giant = b;
  for (unsigned i = 0; i < middle; i++) {
    auto it = mp.find(giant);
    if (it != mp.end()) return cnt + i * middle + it->second;
    giant = 1ull * giant * inv % mod;
  }
  return -1;  // no solution
}