// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/frequency_table_of_tree_distance
// competitive-verifier: TLE 2
// competitive-verifier: MLE 64
// 木の重心分解
#include <iostream>
#include <functional>
#include <queue>
#include "src/FFT/Polynomial.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Poly= Polynomial<long long>;
 int N;
 cin >> N;
 vector<int> tree[N];
 for (int i= 0; i < N - 1; i++) {
  int u, v;
  cin >> u >> v;
  tree[u].push_back(v);
  tree[v].push_back(u);
 }

 bool used[N];
 fill(used, used + N, false);
 int sz[N];
 function<int(int, int)> sz_dfs= [&](int v, int p) {
  sz[v]= 1;
  for (int u: tree[v])
   if (u != p && !used[u]) sz[v]+= sz_dfs(u, v);
  return sz[v];
 };
 function<int(int, int, int)> centroid= [&](int v, int p, int mid) {
  for (int u: tree[v])
   if (u != p && !used[u] && sz[u] > mid) return centroid(u, v, mid);
  return v;
 };

 Poly ans(N + 1, 0);
 function<void(int)> dfs= [&](int rt) {
  int c= centroid(rt, -1, sz_dfs(rt, -1) / 2);
  used[c]= true;
  Poly cnt({1});
  for (int ch: tree[c])
   if (!used[ch]) {
    queue<tuple<int, int, int>> que;
    que.emplace(ch, c, 1);
    Poly num;
    while (!que.empty()) {
     int v, p, d;
     tie(v, p, d)= que.front();
     que.pop();
     if (d >= (int)num.size()) num.resize(d + 1, 0);
     num[d]+= 1;
     for (int u: tree[v])
      if (u != p && !used[u]) que.emplace(u, v, d + 1);
    }
    cnt+= num;
    ans-= num * num;
   }
  ans+= cnt * cnt;
  for (int next: tree[c])
   if (!used[next]) dfs(next);
 };
 dfs(0);
 ans/= 2;
 for (int i= 1; i < N; i++) cout << (i > 1 ? " " : "") << ans[i];
 cout << '\n';
 return 0;
}