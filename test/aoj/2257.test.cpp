// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2257
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>
#include "src/String/AhoCorasick.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
int main() {
 ios::sync_with_stdio(0);
 cin.tie(0);
 using Mint= ModInt<int(1e9 + 7)>;
 for (int N, M, K; cin >> N >> M >> K && N;) {
  map<string, int> s2i;
  vector<string> i2s= {""};
  vector<vector<int>> adj(1);
  for (int i= 0; i < N; i++) {
   string from, to;
   cin >> from >> to;
   if (!s2i.count(from)) {
    i2s.push_back(from);
    s2i[from]= adj.size();
    adj.push_back({});
   }
   if (!s2i.count(to)) {
    i2s.push_back(to);
    s2i[to]= adj.size();
    adj.push_back({});
   }
   int f= s2i[from], t= s2i[to];
   adj[f].push_back(t);
  }
  int n= adj.size();
  for (int i= 1; i < n; i++) adj[0].push_back(i);
  vector<string> seasonword(K);
  for (int i= 0; i < K; i++) cin >> seasonword[i];
  AhoCorasick ac(seasonword);
  unordered_map<uint64_t, Mint> memo;
  auto dfs= [&](auto self, int v, int l, bool kigo, int s) -> Mint {
   if (l > M) return 0;
   if (l == M) return kigo;
   uint64_t tmp= (uint64_t(s * 501 + l) * 2 + kigo) * 501 + v;
   if (auto it= memo.find(tmp); it != memo.end()) return it->second;
   Mint ret= 0;
   for (int u: adj[v]) {
    string us= i2s[u];
    int m= l + us.length();
    if (m > M) continue;
    int t= s;
    int kigo_num= kigo;
    for (char c: us) t= ac.transition(t, c), kigo_num+= ac.matched_patterns(t).size();
    if (kigo_num > 1) continue;
    ret+= self(self, u, m, kigo_num, t);
   }
   return memo[tmp]= ret;
  };
  cout << dfs(dfs, 0, 0, 0, ac.initial_state()) << '\n';
 }
 return 0;
}