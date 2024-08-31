// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1750
// competitive-verifier: TLE 0.5
// 疎行列の行列累乗
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/LinearAlgebra/MinimalPolynomial.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 using Vec= Vector<Mint>;
 long long N, M, T;
 cin >> N >> M >> T;
 int s[M], t[M];
 for (int i= 0; i < M; ++i) cin >> s[i] >> t[i];
 auto f= [&](const Vec& v) {
  Vec ret(N);
  for (int i= 0; i < M; ++i) ret[s[i]]+= v[t[i]], ret[t[i]]+= v[s[i]];
  return ret;
 };
 Vector<Mint> vec(N);
 vec[0]= 1;
 cout << MinimalPolynomial(f, vec).pow(T)[0] << '\n';
 return 0;
}