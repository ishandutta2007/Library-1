// competitive-verifier: STANDALONE

// https://www.codechef.com/problems/CZMG17
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
 int A[N], B[N], C[N], D[N - 1];
 for (int i= 0; i < N; ++i) scin >> A[i];
 for (int i= 0; i < N; ++i) scin >> B[i];
 for (int i= 0; i < N; ++i) scin >> C[i];
 for (int i= 0; i < N - 1; ++i) scin >> D[i];
 PiecewiseLinearConvex<int> f;
 f.add_inf(), f.add_inf(true);
 for (int i= 0; i < N; ++i) {
  if (i) f.add_ramp(-D[i - 1], 0);
  f.shift(-A[i]);
  f.add_linear(-C[i]);
  f.chmin_slide_win(0, B[i]);
  f.add_linear(C[i]);
  f.add_inf(true);
 }
 scout << f(0).value_or(-1) << '\n';
 return 0;
}
}
namespace TEST_conj {
signed main(stringstream& scin, stringstream& scout) {
 int N;
 scin >> N;
 int A[N], B[N], C[N], D[N - 1];
 for (int i= 0; i < N; ++i) scin >> A[i];
 for (int i= 0; i < N; ++i) scin >> B[i];
 for (int i= 0; i < N; ++i) scin >> C[i];
 for (int i= 0; i < N - 1; ++i) scin >> D[i];
 PiecewiseLinearConvex<int> f;
 for (int i= 0; i < N; ++i) {
  if (i) f.chmin_slide_win(-D[i - 1], 0);
  f.add_linear(-A[i]);
  f.add_ramp(B[i], C[i]);
  f.chmin_cum();
 }
 scout << -f.min().value_or(1) << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "3\n2 3 2\n1 2 5\n2 3 1\n1 1\n", "14\n"));

 assert(test(TEST_conj::main, "3\n2 3 2\n1 2 5\n2 3 1\n1 1\n", "14\n"));
 return 0;
}