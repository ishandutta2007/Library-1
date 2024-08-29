// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/exp_of_formal_power_series_sparse
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/sparse_fps.hpp"
using namespace std;
int main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 long long N, K;
 cin >> N >> K;
 vector<Mint> a(N);
 for (int i; K--;) cin >> i >> a[i];
 auto ans= sfps::exp(a, N - 1);
 for (int i= 0; i < N; i++) cout << ans[i] << " \n"[i == N - 1];
 return 0;
}