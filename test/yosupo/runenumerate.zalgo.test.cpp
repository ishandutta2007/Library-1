// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/runenumerate
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <string>
#include "src/String/z_algorithm.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 string S;
 cin >> S;
 auto ans= run_enumerate(S);
 cout << ans.size() << '\n';
 for (auto [t, l, r]: ans) cout << t << " " << l << " " << r << '\n';
 return 0;
}