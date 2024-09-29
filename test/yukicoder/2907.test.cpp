// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/2907
// competitive-verifier: TLE 2
// competitive-verifier: MLE 64
// Nimber inv
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/Nimber.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 Nimber::init();
 int N, T;
 cin >> N >> T;
 vector H(N, vector<Nimber>(18));
 for (int i= 0; i < T; ++i)
  for (int j= 0; j < N; ++j) {
   long long x;
   cin >> x;
   H[j][i]= x - 1;
  }
 Mint ans= 0, pw= Mint(2).pow(64);
 auto dfs= [&](auto &&dfs, int k, int n, int s, vector<vector<Nimber>> B) -> void {
  if (k == N) {
   int r= 0;
   for (int i= N; i--;) r+= B[i][i] == 1;
   Mint x= pw.pow(n - r);
   if ((N - n) & 1) ans-= x;
   else ans+= x;
   return;
  }
  dfs(dfs, k + 1, n, s, B);
  auto X= H[k];
  for (int i= 0; i < N; ++i) {
   if (B[i][i] == 0) {
    if (X[i] == 0) continue;
    Nimber iv= Nimber(1) / X[i];
    X[i]= 1;
    for (int j= i + 1; j < 18; ++j) X[j]*= iv;
    B[i]= X;
    break;
   }
   Nimber m= X[i];
   X[i]= 0;
   for (int j= i + 1; j < 18; ++j) X[j]-= m * B[i][j];
  }
  dfs(dfs, k + 1, n + 1, s | (1 << k), B);
 };
 dfs(dfs, 0, 0, 0, vector(N, vector<Nimber>(18)));
 cout << ans << '\n';
 return 0;
}