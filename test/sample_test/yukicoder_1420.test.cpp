// competitive-verifier: STANDALONE

// https://yukicoder.me/problems/no/1420
// sp judge
// ポテンシャルUF
#include <sstream>
#include <string>
#include <cassert>
#include "src/Math/Nimber.hpp"
#include "src/DataStructure/UnionFind_Potentialized.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 if (scout.str() == "-1\n") return scout.str() == expected;
 stringstream scin2(in);
 int N, M;
 scin2 >> N >> M;
 int A[M], B[M], Y[M];
 for (int i= 0; i < M; ++i) scin2 >> A[i] >> B[i] >> Y[i];
 int ans[N];
 for (int i= 0; i < N; ++i) scout >> ans[i];
 for (int i= 0; i < M; ++i)
  if ((ans[A[i] - 1] ^ ans[B[i] - 1]) != Y[i]) return false;
 return true;
}
namespace TEST {
signed main(stringstream& scin, stringstream& scout) {
 Nimber::init();
 int N, M;
 scin >> N >> M;
 UnionFind_Potentialized<Nimber> uf(N);
 bool isok= true;
 for (int i= 0; i < M; ++i) {
  int A, B;
  Nimber Y;
  scin >> A >> B >> Y;
  --A, --B;
  isok&= uf.unite(A, B, Y);
 }
 if (isok)
  for (int i= 0; i < N; ++i) scout << uf.potential(i) << '\n';
 else scout << -1 << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "3 3\n1 2\n3\n2 3\n5\n1 3\n6\n", "2\n1\n4\n"));
 assert(test(TEST::main, "2 2\n1 2\n1\n1 2\n2\n", "-1\n"));
 return 0;
}