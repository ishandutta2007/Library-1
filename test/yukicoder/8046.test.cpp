// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/8046
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 512
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/fps_inv.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int K;
 cin >> K;
 int N;
 cin >> N;
 using Mint= ModInt<int(1e9 + 7)>;
 vector<Mint> f(1e5 + 10, 0);
 for (int i= 0; i < N; i++) {
  int x;
  cin >> x, f[x]= -1;
 }
 f[0]= 1, f.resize(K + 1);
 auto ans= inv<Mint, 1 << 20>(f);
 cout << ans[K] << '\n';
 return 0;
}