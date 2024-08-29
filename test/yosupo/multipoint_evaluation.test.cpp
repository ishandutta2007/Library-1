// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/multipoint_evaluation
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/SubProductTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N, M;
 cin >> N >> M;
 std::vector<Mint> c(N), p(M);
 for (int i= 0; i < N; i++) cin >> c[i];
 for (int i= 0; i < M; i++) cin >> p[i];
 auto ans= SubProductTree(p).multi_eval(c);
 for (int i= 0; i < M; i++) cout << ans[i] << " \n"[i == M - 1];
 return 0;
}