// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc310/tasks/abc310_g
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Misc/Period.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N;
 long long K;
 cin >> N >> K;
 vector<int> A(N), B(N);
 for (int i= 0; i < N; ++i) cin >> A[i], --A[i];
 for (int i= 0; i < N; ++i) cin >> B[i];
 Period p(A);
 vector<Mint> sum(N + 1);
 for (int i= N; i--;) {
  auto [p1, p2, c, p3]= p.path(i, K);
  for (auto [l, r]: p1) sum[l]+= B[i], sum[r]-= B[i];
  for (auto [l, r]: p2) sum[l]+= Mint(c) * B[i], sum[r]-= Mint(c) * B[i];
  for (auto [l, r]: p3) sum[l]+= B[i], sum[r]-= B[i];
 }
 for (int i= 0; i < N; ++i) sum[i + 1]+= sum[i];
 for (int i= N; i--;) sum[p(i)]-= B[i];
 Mint iv= Mint(1) / K;
 for (int i= 0; i < N; ++i) cout << sum[p(i)] * iv << " \n"[i + 1 == N];
 return 0;
}