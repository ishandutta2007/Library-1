#define PROBLEM "https://yukicoder.me/problems/no/1075"
#include <iostream>
#include <array>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 int N, K;
 cin >> N >> K;
 Graph g(N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i], --g[i];
 g.build(N, 0);
 HeavyLightDecomposition tree(g, 0);
 using Data= vector<Mint>;
 auto put_edge= [&](int, int, const Data &d) { return d; };
 auto op= [&](Data l, const Data &r) {
  for (int i= K; i--;) l[i]*= r[i];
  return l;
 };
 auto put_vertex= [&](int, Data d) {
  for (int i= 1; i < K; ++i) d[i]+= d[i - 1];
  return d;
 };
 Rerooting<Data> dp(g, tree, put_edge, op, Data(K, 1), put_vertex);
 Mint ans= 0;
 for (int i= 0; i < N; ++i) {
  Data a(K, 1);
  for (int e: g(i)) {
   int j= g[e] - i;
   Data b= dp(i, j);
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