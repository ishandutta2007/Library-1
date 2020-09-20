/**
 * @title 離散対数
 * @category 数学
 * O(mod^(1/2))
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

int discrete_log(unsigned a, unsigned b, unsigned mod) {
  if (a == 0) return b == 0 ? (mod == 1 ? 0 : 1) : (b == 1 ? 0 : -1);
  auto mod_inv = [](unsigned x, unsigned mod) {
    int a = x, b = mod, u = 1, v = 0, t;
    while (b) t = a / b, swap(a -= t * b, b), swap(u -= t * v, v);
    return u >= 0 ? u % mod : (mod - (-u) % mod);
  };
  unsigned cnt = 0;
  for (unsigned g; (g = __gcd(a, mod)) != 1; cnt++) {
    if ((b == 1) || (mod == 1)) return cnt;
    if (b % g != 0) return -1;  // no solution
    b /= g, mod /= g;
    b = 1ull * mod_inv(a / g, mod) * b % mod;
  }
  unsigned middle = ceil(sqrt(mod));
  unordered_map<unsigned int, unsigned int> mp;
  unsigned int baby = 1;
  for (unsigned i = 0; i < middle; i++, baby = 1ull * baby * a % mod)
    mp[baby] = i;
  unsigned int inv = mod_inv(baby, mod);
  unsigned int giant = b;
  for (unsigned i = 0; i < middle; i++) {
    auto it = mp.find(giant);
    if (it != mp.end()) return cnt + i * middle + it->second;
    giant = 1ull * giant * inv % mod;
  }
  return -1;  // no solution
}