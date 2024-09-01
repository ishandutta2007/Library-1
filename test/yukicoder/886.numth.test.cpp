// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/886
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 128
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/tables.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9 + 7)>;
 int H, W;
 cin >> H >> W;
 if (H < W) swap(H, W);
 vector<Mint> h(H + 1), w(H + 1);
 for (int i= 1; i < H; i++) h[i]= H - i;
 for (int i= 1; i < W; i++) w[i]= W - i;
 auto a= gcd_convolve(h, w);
 Mint HH(H), WW(W);
 cout << a[1] * 2 + WW * (HH - 1) + HH * (WW - 1) << '\n';
 return 0;
}
