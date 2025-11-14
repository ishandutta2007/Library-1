// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc179/tasks/abc179_d
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/sparse_fps.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N, K;
 cin >> N >> K;
 vector<Mint> num= {1, -1}, den(N + 2);
 den[0]= 1, den[1]= -1;
 for (int i= 0; i < K; ++i) {
  int L, R;
  cin >> L >> R;
  den[L]-= 1;
  den[R + 1]+= 1;
 }
 cout << sfps::div(num, den, N - 1)[N - 1] << '\n';
 return 0;
}