// competitive-verifier: STANDALONE

// https://yukicoder.me/problems/no/1421
// sp judge
// あえて Nimber
#include <sstream>
#include <string>
#include <cassert>
#include "src/Math/Nimber.hpp"
#include "src/LinearAlgebra/LU_Decomposition.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 if (scout.str() == "-1\n") return scout.str() == expected;
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
 Nimber::init();
 int N, M;
 scin >> N >> M;
 Matrix<Nimber> m(M, N);
 Vector<Nimber> Y(M);
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
 auto sol= LU_Decomposition(m).linear_equations(Y);
 if (sol) {
  for (int i= 0; i < N; ++i) scout << sol[i] << '\n';
 } else scout << -1 << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "3 3\n2\n1 2\n3\n2\n2 3\n5\n1\n3\n4\n", "2\n1\n4\n"));
 assert(test(TEST::main, "2 3\n1\n1\n5\n1\n2\n5\n2\n1 2\n5\n", "-1\n"));
 return 0;
}