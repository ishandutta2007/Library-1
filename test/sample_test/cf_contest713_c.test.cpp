// competitive-verifier: STANDALONE

// https://codeforces.com/contest/713/problem/C
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
  int a;
  scin >> a;
  f.chmin_cum();
  f.shift(1);
  f.add_abs(1, a);
 }
 scout << f.min().value() << '\n';
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
  int a;
  scin >> a;
  f.add_inf(true);
  f.add_linear(1);
  f.add_linear(-a);
  f.chmin_slide_win(-1, 1);
  f.add_linear(a);
 }
 scout << -f(0).value() << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "7\n2 1 5 11 5 9 11\n", "9\n"));
 assert(test(TEST::main, "5\n5 4 3 2 1\n", "12\n"));

 assert(test(TEST_conj::main, "7\n2 1 5 11 5 9 11\n", "9\n"));
 assert(test(TEST_conj::main, "5\n5 4 3 2 1\n", "12\n"));

 return 0;
}