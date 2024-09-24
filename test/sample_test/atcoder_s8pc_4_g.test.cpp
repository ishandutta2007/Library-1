// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/s8pc-4/tasks/s8pc_4_g
// 遅延伝搬
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <vector>
#include <array>
#include <set>
#include "src/DataStructure/KDTree.hpp"
using namespace std;
bool test(int (*solve)(stringstream &, stringstream &), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
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
signed main(stringstream &scin, stringstream &scout) {
 int N, Q;
 scin >> N >> Q;
 int a[N];
 vector<vector<int>> adj(N);
 for (int i= 0; i < N; i++) {
  int p;
  scin >> p >> a[i];
  if (p != -1) adj[p].push_back(i);
 }
 vector<array<int, 4>> query(Q);
 int M= N;
 for (int i= 0; i < Q; i++) {
  int type;
  scin >> type;
  if (type == 1) {
   int v, d, x;
   scin >> v >> d >> x;
   query[i]= {type, v, d, x};
  } else if (type == 2) {
   int v, d;
   scin >> v >> d;
   query[i]= {type, v, d, -1};
  } else {
   int pr, ar;
   scin >> pr >> ar;
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
  else if (type == 2) scout << kdt.prod_cuboid(in[v], out[v] - 1, dep[v], dep[v] + d).val << '\n';
  else kdt.set(in[v], dep[v], {x, 1});
 }
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "6 7\n-1 6\n0 5\n0 4\n2 3\n2 2\n1 1\n2 0 1\n1 0 2 1\n2 2 1\n3 3 3\n2 0 3\n3 3 4\n2 1 1\n", "15\n12\n30\n8\n"));
 assert(test(TEST::main, "7 9\n-1 1\n0 5\n0 7\n0 8\n1 3\n4 1\n5 1\n2 1 1\n2 1 2\n1 1 2 3\n1 4 1 1\n2 3 1\n2 0 2\n3 6 1\n3 7 11\n2 0 15\n", "8\n9\n8\n31\n49\n"));
 return 0;
}