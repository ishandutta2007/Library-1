// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/utpc2012/tasks/utpc2012_12
#include <sstream>
#include <string>
#include <cassert>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
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
 vector<int> C(N);
 scin >> C[0];
 vector<vector<int>> tree(N);
 for (int i= 1, P; i < N; i++) scin >> P >> C[i], tree[--P].push_back(i);
 using PLC= PiecewiseLinearConvex<int>;
 auto dfs= [&](auto&& dfs, int v) -> PLC {
  PLC f;
  f.add_abs(1, C[v]);
  for (int u: tree[v]) {
   auto g= dfs(dfs, u);
   g.chmin_cum(), g.shift(1);
   f+= g;
  }
  return f;
 };
 scout << dfs(dfs, 0).min().value() << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "8 6\n1 1\n2 1\n2 3\n1 9\n5 6\n6 6\n6 2\n", "8\n"));
 assert(test(TEST::main, "4 5\n1 5\n2 5\n3 5\n", "4\n"));
 return 0;
}