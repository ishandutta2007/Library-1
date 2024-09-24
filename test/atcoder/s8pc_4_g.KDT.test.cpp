// competitive-verifier: PROBLEM https://atcoder.jp/contests/s8pc_4/tasks/s8pc_4/s8pc_4_g
// competitive-verifier: TLE 4.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/s8pc-4/tasks/s8pc_4_g
// 遅延伝搬
#include <iostream>
#include <vector>
#include <array>
#include <tuple>
#include "src/DataStructure/KDTree.hpp"
using namespace std;
struct RaddsumQ {
 struct T {
  long long val;
  int sz;
 };
 using E= long long;
 static T ti() { return {0, 0}; }
 static T op(const T &l, const T &r) { return {l.val + r.val, l.sz + r.sz}; }
 static void mp(T &v, E x) { v.val+= x * v.sz; }
 static void cp(E &pre, E suf) { pre+= suf; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, Q;
 cin >> N >> Q;
 int a[N];
 vector<vector<int>> adj(N);
 for (int i= 0; i < N; i++) {
  int p;
  cin >> p >> a[i];
  if (p != -1) adj[p].push_back(i);
 }
 vector<array<int, 4>> query(Q);
 int M= N;
 for (int i= 0; i < Q; i++) {
  int type;
  cin >> type;
  if (type == 1) {
   int v, d, x;
   cin >> v >> d >> x;
   query[i]= {type, v, d, x};
  } else if (type == 2) {
   int v, d;
   cin >> v >> d;
   query[i]= {type, v, d, -1};
  } else {
   int pr, ar;
   cin >> pr >> ar;
   adj[pr].push_back(M);
   query[i]= {type, M++, -1, ar};
   adj.resize(M);
  }
 }
 int in[M], out[M], dep[M];
 auto dfs= [&adj, &in, &out, &dep](auto self, int v, int d) -> void {
  static int t= 0;
  dep[v]= d, in[v]= t++;
  for (int u: adj[v]) self(self, u, d + 1);
  out[v]= t;
 };
 dfs(dfs, 0, 0);
 vector<tuple<int, int, typename RaddsumQ::T>> vec(M);
 for (int i= 0; i < N; i++) vec[i]= {in[i], dep[i], {a[i], 1}};
 for (int i= N; i < M; i++) vec[i]= {in[i], dep[i], {0, 0}};
 KDTree<int, 2, RaddsumQ> kdt(vec);
 for (auto [type, v, d, x]: query) {
  if (type == 1) kdt.apply_cuboid(in[v], out[v] - 1, dep[v], dep[v] + d, x);
  else if (type == 2) cout << kdt.prod_cuboid(in[v], out[v] - 1, dep[v], dep[v] + d).val << '\n';
  else kdt.set(in[v], dep[v], {x, 1});
 }
 return 0;
}