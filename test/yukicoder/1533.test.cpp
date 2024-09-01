// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1533
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/Math/sparse_fps.hpp"
using namespace std;
int main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 int N, M, K;
 cin >> N >> M >> K;
 if (M == 1 && K == 1) {
  cout << 0 << '\n';
  return 0;
 }
 if (M == K) M--;
 int s= 0;
 for (int i= -M; i <= M; i++) s+= abs(i) != K;
 vector<Mint> f(M + M + 1, 1);
 if (K <= M) f[M - K]= f[M + K]= 0;
 int lim= N * M;
 auto F= sfps::pow(f, N + 2, lim);
 f[M]-= s;
 F= sfps::div(F, f, lim), F= sfps::div(F, f, lim);
 for (int i= 1; i < lim; i++) F[i]+= F[i - 1];
 cout << F[lim - 1] << '\n';
 return 0;
}