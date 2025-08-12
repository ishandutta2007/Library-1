// competitive-verifier: PROBLEM https://loj.ac/p/154
// competitive-verifier: TLE 3.5
// competitive-verifier: MLE 512

#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/set_power_series.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 int n, m, k;
 cin >> n >> m >> k;
 vector<Mint> f(1 << n, 0);
 for (int s; m--;) cin >> s, f[s]+= 1;
 vector<Mint> F(k + 1, 1);
 cout << sps::egf_comp(f, F)[(1 << n) - 1] << '\n';
 return 0;
}