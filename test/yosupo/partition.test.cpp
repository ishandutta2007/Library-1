// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/partition_function
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 512
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/FFT/sequences.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N;
 cin >> N;
 auto ans= partition<Mint>(N);
 for (int i= 0; i <= N; i++) cout << ans[i] << " \n"[i == N];
 return 0;
}