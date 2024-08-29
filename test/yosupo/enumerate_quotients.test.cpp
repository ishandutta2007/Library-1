// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/enumerate_quotients
// competitive-verifier: TLE 0.5
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/enumerate_quotients.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 long long N;
 cin >> N;
 auto ans= enumerate_quotients(N);
 int k= ans.size();
 cout << k << '\n';
 for (int i= 0; i < k; ++i) cout << get<0>(ans[i]) << " \n"[i == k - 1];
 return 0;
}