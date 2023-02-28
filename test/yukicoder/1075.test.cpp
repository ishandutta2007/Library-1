#define PROBLEM "https://yukicoder.me/problems/no/1075"
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
 tree.build(0);
 using Data= vector<Mint>;
 auto f_ee= [&](Data l, const Data &r) {
  for (int i= 0; i < K; ++i) l[i]*= r[i];
  return l;
 };
 auto f_ve= [&](const Data &d, int, auto) { return d; };
 auto f_ev= [&](Data d, int) {
  for (int i= 1; i < K; ++i) d[i]+= d[i - 1];
  return d;
 };
 auto dp= rerooting<Data>(tree, f_ee, f_ve, f_ev, Data(K, 1));
 Mint ans= 0;
 for (int i= 0; i < N; ++i) {
  Data a(K, 1);
  for (int j: tree[i]) {
   Data b= dp.get(i, j);
   if (j == tree.parent(i)) {
    for (int k= 1; k < K; ++k) a[k]*= b[k - 1];
    a[0]= 0;
   } else
    for (int k= 0; k < K; ++k) a[k]*= b[k];
  }
  for (int k= 1; k < K; ++k) a[k]+= a[k - 1];
  ans+= a[K - 1];
 }
 cout << ans << '\n';
 return 0;
}