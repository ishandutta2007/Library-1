// competitive-verifier: PROBLEM https://atcoder.jp/contests/arc060/tasks/arc060_f
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/arc060/tasks/arc060_d
#include <iostream>
#include <string>
#include <algorithm>
#include "src/String/z_algorithm.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 string w;
 cin >> w;
 int n= w.length();
 auto p= z_to_kmp(z_algorithm(w));
 for (int i= 0; i <= n; ++i) p[i]= i - p[i];
 if (n % p[n] != 0 || n == p[n]) return cout << 1 << '\n' << 1 << '\n', 0;
 if (p[n] == 1) return cout << n << '\n' << 1 << '\n', 0;
 cout << 2 << '\n';
 reverse(w.begin(), w.end());
 auto q= z_to_kmp(z_algorithm(w));
 for (int i= 0; i <= n; ++i) q[i]= i - q[i];
 int ans= 0;
 for (int i= 1; i < n; ++i) ans+= (i % p[i] != 0 || i == p[i]) && ((n - i) % q[n - i] != 0 || (n - i) == q[n - i]);
 cout << ans << '\n';
 return 0;
}