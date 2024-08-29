// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc276/tasks/abc276_g
// competitive-verifier: TLE 0.5
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/Math/sparse_fps.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N, M;
 cin >> N >> M;
 if (M-= --N; M < 0) {
  cout << 0 << '\n';
  return 0;
 }
 auto F= sfps::pow_of_div<Mint>({1, 1}, {1, 0, 0, -1}, N, M);
 for (int i= 1; i <= M; i++) F[i]+= F[i - 1];
 for (int i= 1; i <= M; i++) F[i]+= F[i - 1];
 cout << F[M] << '\n';
 return 0;
}