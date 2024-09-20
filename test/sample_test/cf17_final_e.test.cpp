// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/cf17-final/tasks/cf17_final_e

#include <sstream>
#include <string>
#include <cassert>
#include "src/Math/ModInt.hpp"
#include "src/Graph/incidence_matrix_equation.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
signed main(stringstream& scin, stringstream& scout) {
 using Mint= ModInt<26>;
 string S;
 scin >> S;
 int N, M= S.length() + 1;
 scin >> N;
 Graph g((M + 1) / 2);
 for (int i= 0; i < N; i++) {
  int L, R;
  scin >> L >> R;
  --L;
  if (2 * L >= M) L= M - 1 - L;
  if (2 * R >= M) R= M - 1 - R;
  g.add_edge(L, R);
 }
 vector<Mint> b((M + 1) / 2);
 b[0]= S[0] - S.back();
 for (int i= 1; i < M / 2; ++i) b[i]= (S[i] - S[i - 1]) + (S[M - 1 - i] - S[M - 2 - i]);
 if (M & 1) b[M / 2]= S[M / 2] - S[M / 2 - 1];
 scout << (incidence_matrix_equation(g, b).empty() ? "NO" : "YES") << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "bixzja\n2\n2 3\n3 6\n", "YES\n"));
 assert(test(TEST::main, "abc\n1\n2 2\n", "NO\n"));
 assert(test(TEST::main, "cassert\n4\n1 2\n3 4\n1 1\n2 2\n", "YES\n"));
 return 0;
}