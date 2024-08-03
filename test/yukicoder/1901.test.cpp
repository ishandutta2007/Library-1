// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1901
#include <iostream>
#include <vector>
#include "src/Math/set_power_series.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int n;
 cin >> n;
 int N= 1 << n;
 vector<int> a[32], b[32];
 for (int i= 32; i--;) a[i].resize(N), b[i].resize(N);
 for (int i= 0; i < N; i++)
  for (int j= 0; j < 32; ++j) cin >> a[j][i];
 for (int i= 0; i < N; i++)
  for (int j= 0; j < 32; ++j) cin >> b[j][i];
 for (int i= 32; i--;) sps::hadamard(a[i]), sps::hadamard(b[i]);
 vector<int> c[64];
 for (int i= 64; i--;) c[i].resize(N);
 for (int i= 32; i--;)
  for (int j= 32; j--;)
   for (int k= 0; k < N; ++k) c[i + j][k]+= a[i][k] * b[j][k];
 for (int i= 64; i--;) sps::hadamard(c[i]);
 for (int i= 0; i < N; ++i)
  for (int j= 0; j < 64; ++j) cout << ((c[j][i] >> n) & 1) << " \n"[j == 63];
 return 0;
}