// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/sparse_matrix_det
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/LinearAlgebra/MinimalPolynomial.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 using Vec= Vector<Mint>;
 int N, K;
 cin >> N >> K;
 int a[K], b[K], c[K];
 for (int i= 0; i < K; i++) cin >> a[i] >> b[i] >> c[i];
 auto f= [&](const Vec& v) {
  Vec ret(N);
  for (int i= 0; i < K; i++) ret[a[i]]+= v[b[i]] * c[i];
  return ret;
 };
 cout << linear_map_det<Mint>(f, N) << '\n';
 return 0;
}