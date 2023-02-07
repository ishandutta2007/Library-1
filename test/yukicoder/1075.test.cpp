#define PROBLEM "https://yukicoder.me/problems/no/1075"
// 森
#include <iostream>
#include <array>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Graph/rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 int N, K;
 cin >> N >> K;
 Tree tree(N);
 for (int i= 0; i < N - 1; ++i) {
  int a, b;
  cin >> a >> b;
  tree.add_edge(--a, --b);
 }
 tree.build();
 using Data= vector<Mint>;
 using DD= array<Data, 2>;
 auto f_ee= [&](const DD &l, const DD &r) {
  DD ret{Data(K), Data(K)};
  for (int i= 0; i < K; ++i) ret[0][i]= l[0][i] * r[0][i];
  for (int i= 0; i < K; ++i) ret[1][i]= l[1][i] * r[1][i];
  return ret;
 };
 auto f_ve= [&](DD d, int p, auto e) {
  if (p > e.to) {
   for (int i= K; --i;) d[1][i]= d[0][i - 1];
   d[1][0]= 0;
  }
  return d;
 };
 auto f_ev= [&](DD d, int) {
  for (int i= 1; i < K; ++i) d[0][i]+= d[0][i - 1];
  for (int i= 1; i < K; ++i) d[1][i]+= d[1][i - 1];
  return d;
 };
 auto dp= rerooting<DD>(tree, f_ee, f_ve, f_ev, DD{Data(K, 1), Data(K, 1)});
 Mint ans= 0;
 for (const auto &x: dp) ans+= x[1][K - 1];
 cout << ans << '\n';
 return 0;
}