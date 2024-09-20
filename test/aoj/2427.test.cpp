// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2427
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// ポテンシャルUF
#include <iostream>
#include <vector>
#include <unordered_map>
#include "src/DataStructure/UnionFind_Potentialized.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 long long dist;
 cin >> dist;
 int n;
 cin >> n;
 vector<long long> S(n);
 for (int i= 0; i < n; ++i) cin >> S[i];
 int m;
 cin >> m;
 vector<long long> D(m);
 for (int i= 0; i < m; ++i) cin >> D[i];
 vector<pair<int, int>> ps;
 for (int i= n; i--;)
  for (int j= i; j--;) ps.emplace_back(j, i);
 int N= ps.size();
 long long ans= 1ll << 60;
 vector<long long> diff(N);
 auto dfs= [&](auto &&dfs, int i) -> void {
  if (i == N) {
   UnionFind_Potentialized<long long> uf(n);
   for (int i= N; i--;) {
    auto [a, b]= ps[i];
    if (diff[i] < 0) continue;
    if (!uf.unite(b, a, diff[i])) return;
   }
   vector<long long> s(n), g(n);
   for (int i= n; i--;) s[i]= uf.potential(i), g[i]= s[i] + S[i] * dist;
   for (auto [a, b]: ps) {
    if (!uf.connected(a, b)) return;
    if (uf.diff(b, a) < 1) return;
    if (g[a] > g[b]) {
     bool ok= false;
     for (int k= m; k--;) ok|= s[a] + S[a] * D[k] == s[b] + S[b] * D[k];
     if (!ok) return;
    }
   }
   for (int k= m; k--;) {
    unordered_map<long long, int> memo;
    for (int l= n; l--;) memo[s[l] + S[l] * D[k]]++;
    for (auto [_, cnt]: memo)
     if (cnt > 2) return;
   }
   ans= min(ans, *max_element(g.begin(), g.end()) - s[0]);
   return;
  }
  auto [a, b]= ps[i];
  long long ga= S[a] * dist, gb= S[b] * dist;
  if (gb + 1 >= ga) diff[i]= 1, dfs(dfs, i + 1);
  if (gb + 1 <= ga) diff[i]= ga - gb, dfs(dfs, i + 1);
  if (S[a] > S[b])
   for (int k= m; k--;) diff[i]= (S[a] - S[b]) * D[k], dfs(dfs, i + 1);
  diff[i]= -1, dfs(dfs, i + 1);
 };
 dfs(dfs, 0);
 cout << ans << '\n';
 return 0;
}