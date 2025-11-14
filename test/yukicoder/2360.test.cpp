// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/2360
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <string>
#include "src/Math/ModInt.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N;
 cin >> N;
 vector<string> A(N);
 for (int i= 0; i < N; ++i) cin >> A[i];
 Graph g(N, N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i], --g[i];
 using T= pair<int, Mint>;
 auto put_edge= [&](int, int, const T &d) { return d; };
 auto op= [&](const T &l, const T &r) { return T{l.first + r.first, l.second + r.second}; };
 auto put_vertex= [&](int v, T d) {
  ++d.first;
  d.second*= Mint(10).pow(A[v].length());
  d.second+= Mint(stoll(A[v])) * d.first;
  return d;
 };
 Mint ans= 0;
 for (auto [_, x]: Rerooting<T>(g, put_edge, op, T(0, 0), put_vertex)) ans+= x;
 cout << ans << "\n";
 return 0;
}