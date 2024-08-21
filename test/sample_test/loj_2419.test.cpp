// competitive-verifier: STANDALONE

// https://loj.ac/p/2419
// オリジナル: https://usaco.org/index.php?page=viewproblem2&cpid=650

#include <sstream>
#include <string>
#include <cassert>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
#include "src/Optimization/fibonacci_search.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
signed main(stringstream& scin, stringstream& scout) {
 int N, X, Y, Z;
 scin >> N >> X >> Y >> Z;
 PiecewiseLinearConvex<int> f;
 f.add_inf(), f.add_inf(true);
 for (int i= 0; i < N; ++i) {
  int A, B;
  scin >> A >> B;
  f.shift(A - B);
  f.add_linear(Y);
  f.chmin_cum(true);
  f.add_linear(-X - Y);
  f.chmin_cum();
  f.add_linear(X);
  f.add_abs(Z, 0);
 }
 scout << f(0).value() << '\n';
 return 0;
}
}
namespace TEST_conj {
signed main(stringstream& scin, stringstream& scout) {
 int N, X, Y, Z;
 scin >> N >> X >> Y >> Z;
 PiecewiseLinearConvex<int> f;
 for (int i= 0; i < N; ++i) {
  int A, B;
  scin >> A >> B;
  f.add_linear(A - B);
  f.add_inf(false, -Y);
  f.add_inf(true, X);
  f.chmin_slide_win(-Z, Z);
 }
 scout << -f.min().value() << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "4 100 200 1\n1 4\n2 3\n3 2\n4 0\n", "210\n"));

 assert(test(TEST_conj::main, "4 100 200 1\n1 4\n2 3\n3 2\n4 0\n", "210\n"));
 return 0;
}