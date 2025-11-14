// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1142
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/set_power_series.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9) + 7>;
 int N, M, K;
 cin >> N >> M >> K;
 vector<Mint> A(1024), B(1024);
 A[0]= B[0]= 1;
 for (int i= 0, x= 0, a; i < N; ++i) cin >> a, A[x^= a]+= 1;
 for (int i= 0, x= 0, b; i < M; ++i) cin >> b, B[x^= b]+= 1;
 A= sps::xor_convolve(A, A), B= sps::xor_convolve(B, B);
 A[0]-= N + 1, B[0]-= M + 1;
 cout << sps::xor_convolve(A, B)[K] / 4 << '\n';
 return 0;
}