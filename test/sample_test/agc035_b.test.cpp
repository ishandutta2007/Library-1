// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/agc035/tasks/agc035_b
// sp judge
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <vector>
#include "src/Graph/incidence_matrix_equation.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 if (scout.str() == "-1\n") return scout.str() == expected;
 stringstream scin2(in);
 int N, M;
 scin2 >> N >> M;
 int A[M], B[M];
 for (int i= 0; i < M; ++i) scin2 >> A[i] >> B[i];
 bool f[M];
 for (int i= 0; i < M; i++) f[i]= 0;
 int od[N];
 for (int i= 0; i < N; i++) od[i]= 0;
 for (int i= 0; i < M; ++i) {
  int C, D;
  scout >> C >> D;
  for (int j= 0; j < M; j++) {
   if ((A[j] == C && B[j] == D) || (A[j] == D && B[j] == C)) {
    if (f[j]) return false;
    f[j]= true;
   }
  }
  ++od[C - 1];
 }
 for (int i= 0; i < M; ++i)
  if (!f[i]) return false;
 for (int i= 0; i < N; ++i)
  if (od[i] & 1) return false;
 return true;
}
namespace TEST {
signed main(stringstream& scin, stringstream& scout) {
 int N, M;
 scin >> N >> M;
 Graph g(N, M);
 vector<bool> b(N, 0);
 for (int i= 0; i < M; ++i) scin >> g[i], --g[i], b[g[i].second]= !b[g[i].second];
 auto sol= incidence_matrix_equation(g, b);
 if (sol.empty()) return scout << -1 << '\n', 0;
 for (int i= 0; i < M; i++) {
  auto [A, B]= g[i];
  if (!sol[i]) swap(A, B);
  scout << A + 1 << " " << B + 1 << '\n';
 }
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "4 4\n1 2\n2 3\n3 4\n4 1\n", "1 2\n1 4\n3 2\n3 4\n"));
 assert(test(TEST::main, "5 5\n1 2\n2 3\n3 4\n2 5\n4 5\n", "-1\n"));
 return 0;
}