// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/6/NTL/2/NTL_2_F
// competitive-verifier: TLE 0.5
#include <iostream>
#include "src/FFT/BigInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 BigInt A, B;
 cin >> A >> B;
 cout << A * B << endl;
 return 0;
}