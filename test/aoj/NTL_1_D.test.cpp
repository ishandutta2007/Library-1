// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/6/NTL/1/NTL_1_D
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/NumberTheory/Factors.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 long long n;
 cin >> n;
 cout << totient(n) << '\n';
 return 0;
}
