// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/arc010/tasks/arc010_4
// 最近傍探索
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <vector>
#include <array>
#include "src/DataStructure/KDTree.hpp"
#include "src/Graph/StronglyConnectedComponents.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
signed main(stringstream& scin, stringstream& scout) {
 int N;
 scin >> N;
 vector<array<int, 2>> f(N);
 for (int i= 0; i < N; ++i) scin >> f[i][0] >> f[i][1];
 int M;
 scin >> M;
 vector<array<int, 2>> s(M);
 for (int i= 0; i < M; ++i) scin >> s[i][0] >> s[i][1];
 KDTree<int, 2> kdt(s);
 vector<long long> lim(N, 1ll << 60);
 auto dist2= [&](int x1, int y1, int x2, int y2) {
  long long dx= x1 - x2, dy= y1 - y2;
  return dx * dx + dy * dy;
 };
 if (M)
  for (int i= 0; i < N; ++i) {
   auto [x, y]= f[i];
   auto [xx, yy]= kdt.nearest_neighbor(x, y);
   lim[i]= dist2(x, y, xx, yy);
  }
 Graph g(N);
 for (int i= N; i--;)
  for (int j= i; j--;) {
   auto d= dist2(f[i][0], f[i][1], f[j][0], f[j][1]);
   if (lim[i] > d) g.add_edge(i, j);
   if (lim[j] > d) g.add_edge(j, i);
  }
 StronglyConnectedComponents scc(g);
 auto dag= scc.dag(g);
 int C= scc.size();
 vector<int> start(C, true);
 auto adj= dag.adjacency_vertex(1);
 for (int i= C; i--;)
  for (int j: adj[i]) start[j]= false;
 int ans= 0;
 for (int i= C; i--;) ans+= start[i];
 scout << ans << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "3\n1 1\n1 2\n2 1\n0\n", "1\n"));
 assert(test(TEST::main, "2\n1 1\n1 2\n1\n2 1\n", "1\n"));
 assert(test(TEST::main, "5\n1 1\n1 2\n2 3\n3 3\n5 3\n2\n2 1\n4 4\n", "2\n"));
 assert(test(TEST::main, "10\n-10 5\n2 9\n-4 4\n10 -9\n8 3\n5 6\n4 -5\n6 8\n-8 10\n-4 -2\n10\n-1 2\n-2 -7\n9 -3\n-5 5\n6 -10\n-10 9\n7 4\n2 1\n-10 1\n-5 2\n", "8\n"));
 return 0;
}