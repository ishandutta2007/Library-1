// competitive-verifier: PROBLEM https://loj.ac/p/2072
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <iostream>
#include <vector>
#include <set>
#include "src/Math/ModInt.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
#include "src/Misc/Pointwise.hpp"
#include "src/Misc/rng.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Graph A(N, N - 1), B(N + 1, N);
 for (auto &e: A) cin >> e, --e;
 for (auto &e: B) cin >> e, --e;
 using Mint= ModInt<(1ll << 61) - 1>;
 using K= Pointwise<Mint, Mint>;
 vector<K> hash(N + 1);
 for (auto &x: hash) x= {rng(2, Mint::mod() - 1), rng(2, Mint::mod() - 1)};
 using T= pair<int, K>;
 auto put_edge= [&](int, int, const T &d) { return T(d.first, d.second + hash[d.first]); };
 auto op= [&](const T &l, const T &r) { return T(max(l.first, r.first), l.second * r.second); };
 auto put_vertex= [&](int, const T &d) { return T(d.first + 1, d.second); };
 Rerooting<T> dp1(A, put_edge, op, T(0, 1), put_vertex), dp2(B, put_edge, op, T(0, 1), put_vertex);
 set<K> S;
 for (auto &&x: dp1) S.insert(x.second);
 int ans= -1;
 auto adj= B.adjacency_vertex(0);
 for (int i= 0; i <= N; ++i)
  if (adj[i].size() == 1)
   if (S.count(dp2(i, adj[i][0]).second)) {
    ans= i + 1;
    break;
   }
 cout << ans << '\n';
 return 0;
}