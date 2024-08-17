// competitive-verifier: STANDALONE

// https://cses.fi/problemset/task/2132/
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
 int n;
 scin >> n;
 PiecewiseLinearConvex<int> f;
 for (int i= 0; i < n; ++i) {
  int x;
  scin >> x;
  f.chmin_cum();
  f.add_abs(1, x);
 }
 scout << f.min() << '\n';
 return 0;
}
}
namespace TEST_conj {
signed main(stringstream& scin, stringstream& scout) {
 int n;
 scin >> n;
 PiecewiseLinearConvex<int> f;
 f.add_inf();
 for (int i= 0; i < n; ++i) {
  int x;
  scin >> x;
  f.add_inf(true);
  f.add_linear(-x);
  f.chmin_slide_win(-1, 1);
  f.add_linear(x);
 }
 scout << -f(0) << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "5\n3 8 5 6 5\n", "4\n"));

 assert(test(TEST_conj::main, "5\n3 8 5 6 5\n", "4\n"));
 return 0;
}