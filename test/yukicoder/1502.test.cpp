// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1502
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// affine合成 非可換群
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/DataStructure/UnionFind_Potentialized.hpp"
#include "src/Math/Algebra.hpp"
using namespace std;
struct M {
 using T= pair<bool, long long>;
 static constexpr T o= {false, 0};
 static T add(const T &a, const T &b) {
  if (b.first) return {!a.first, b.second - a.second};
  else return {a.first, a.second + b.second};
 }
 static T neg(const T &a) { return {a.first, (a.first ? a.second : -a.second)}; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9 + 7)>;
 using G= Algebra<M>;
 int N, M, K;
 cin >> N >> M >> K;
 UnionFind_Potentialized<G> uf(N);
 vector<tuple<int, int, long long>> dat;
 for (int i= 0; i < M; ++i) {
  int X, Y;
  long long Z;
  cin >> X >> Y >> Z, --X, --Y;
  dat.emplace_back(X, Y, Z);
  if (uf.connected(X, Y)) continue;
  uf.unite(X, Y, G(make_pair(true, Z)));
 }
 auto calc= [&](int k) -> Mint {
  long long lb[N], ub[N];
  fill_n(lb, N, -(1LL << 60));
  fill_n(ub, N, 1LL << 60);
  for (int i= N; i--;) {
   int v= uf.leader(i);
   auto [a, b]= uf.potential(i).x;
   if (a) lb[v]= max(lb[v], b - k), ub[v]= min(ub[v], b - 1);
   else lb[v]= max(lb[v], 1 - b), ub[v]= min(ub[v], k - b);
  }
  for (auto &&[X, Y, Z]: dat) {
   auto [xa, xb]= uf.potential(X).x;
   auto [ya, yb]= uf.potential(Y).x;
   long long q= Z - xb - yb;
   if (xa ^ ya) {
    if (q) return 0;
    continue;
   }
   if (q & 1) return 0;
   if (xa) q= -q;
   q/= 2;
   if (q < 0 || k < q) return 0;
   int v= uf.leader(X);
   lb[v]= max(lb[v], q), ub[v]= min(ub[v], q);
  }
  Mint ret= 1;
  for (int i= N; i--;) {
   if (uf.leader(i) != i) continue;
   ret*= max(0ll, ub[i] - lb[i] + 1);
  }
  return ret;
 };
 cout << calc(K) - calc(K - 1) << '\n';
 return 0;
}
