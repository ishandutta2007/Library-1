// competitive-verifier: STANDALONE

// https://yukicoder.me/problems/no/1421
// sp judge
#include <sstream>
#include <string>
#include <cassert>
#include "src/LinearAlgebra/LU_Decomposition.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 if (expected == "-1\n") return scout.str() == expected;
 stringstream scin2(in);
 int N, M;
 scin2 >> N >> M;
 int A[M], Y[M];
 vector<int> B[M];
 for (int i= 0; i < M; ++i) {
  scin2 >> A[i];
  B[i].resize(A[i]);
  for (int j= 0; j < A[i]; ++j) scin2 >> B[i][j], --B[i][j];
  scin2 >> Y[i];
 }
 int ans[N];
 for (int i= 0; i < N; i++) scout >> ans[i];
 for (int i= 0; i < M; ++i) {
  int x= 0;
  for (int j= 0; j < A[i]; ++j) x^= ans[B[i][j]];
  if (x != Y[i]) return false;
 }
 return true;
}
namespace TEST {
signed main(stringstream& scin, stringstream& scout) {
 int N, M;
 scin >> N >> M;
 Matrix<bool> m(M, N);
 int Y[M];
 for (int i= 0; i < M; ++i) {
  int A;
  scin >> A;
  for (int j= 0; j < A; ++j) {
   int B;
   scin >> B, --B;
   m[i][B]= 1;
  }
  scin >> Y[i];
 }
 LU_Decomposition lud(m);
 int ans[N];
 fill_n(ans, N, 0);
 for (int k= 0; k < 30; ++k) {
  Vector<bool> b(M);
  for (int i= 0; i < M; ++i) b[i]= (Y[i] >> k) & 1;
  auto sol= lud.linear_equations(b);
  if (!sol) return scout << -1 << '\n', 0;
  for (int i= 0; i < N; ++i) ans[i]|= sol[i] << k;
 }
 for (int i= 0; i < N; ++i) scout << ans[i] << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "3 3\n2\n1 2\n3\n2\n2 3\n5\n1\n3\n4\n", "2\n1\n4\n"));
 assert(test(TEST::main, "2 3\n1\n1\n5\n1\n2\n5\n2\n1 2\n5\n", "-1\n"));
 return 0;
}