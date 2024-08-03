// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/bostan_mori.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 int d;
 cin >> d;
 std::uint64_t k;
 cin >> k;
 std::vector<Mint> a(d), c(d);
 for (int i= 0; i < d; i++) cin >> a[i];
 for (int i= 0; i < d; i++) cin >> c[i];
 cout << linear_recurrence(c, a, k) << '\n';
 return 0;
}