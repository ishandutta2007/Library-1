// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc248/tasks/abc248_g
// competitive-verifier: TLE 3
// competitive-verifier: MLE 2048
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
#include "src/NumberTheory/ArrayOnDivisors.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int n;
 cin >> n;
 vector<int> A(n);
 Mint sum= 0;
 for (int &a: A) cin >> a, sum+= a;
 Graph g(n, n - 1);
 for (auto &e: g) cin >> e, --e;
 using T= ArrayOnDivisors<int, pair<int, Mint>>;
 auto put_edge= [&](int v, int, const T &dat) {
  T ret(A[v]);
  for (auto &&[d, a]: dat) {
   auto &[l0, l1]= ret[gcd(d, A[v])];
   auto [r0, r1]= a;
   l1+= r1;
   l0+= r0;
  }
  return ret;
 };
 auto op= [&](const T &l, const T &r) {
  if (l.size() == 0) return r;
  if (r.size() == 0) return l;
  auto ret= l;
  for (auto &[d, a]: ret) {
   auto &[l0, l1]= a;
   auto [r0, r1]= r[d];
   l1+= r1;
   l0+= r0;
  }
  return ret;
 };
 auto put_vertex= [&](int v, T dat) {
  if (dat.size() == 0) dat= T(A[v]);
  dat[A[v]].first+= 1;
  for (auto &[d, x]: dat) x.second+= x.first;
  return dat;
 };
 Mint ans= 0;
 for (auto dat: Rerooting<T>(g, put_edge, op, T(), put_vertex))
  for (auto &&[d, x]: dat) ans+= x.second * d;
 ans-= sum, ans/= 2;
 cout << ans << '\n';
 return 0;
}