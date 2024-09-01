// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc172/tasks/abc172_d
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 128
// O(N)
#include <iostream>
#include "src/NumberTheory/tables.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N;
 cin >> N;
 auto d= multiplicative_table<long long>(N, [](long long, short e) { return e + 1; });
 long long ans= 0;
 for (long long i= 1; i <= N; i++) ans+= d[i] * i;
 cout << ans << '\n';
 return 0;
}