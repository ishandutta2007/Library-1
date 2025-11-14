// competitive-verifier: STANDALONE

// https://codeforces.com/contest/866/problem/D
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
 using PLC= PiecewiseLinearConvex<int>;
 PLC f;
 f.add_inf(), f.add_inf(true);
 for (int i= 0; i < N; ++i) {
  int p;
  scin >> p;
  f.add_linear(-p);
  f.chmin_slide_win(-1, 1);
  f.add_linear(p);
  f.add_inf();
 }
 scout << -f(0).value() << '\n';
 return 0;
}
}
namespace TEST_conj {
signed main(stringstream& scin, stringstream& scout) {
 int N;
 scin >> N;
 using PLC= PiecewiseLinearConvex<int>;
 PLC f;
 for (int i= 0; i < N; ++i) {
  int p;
  scin >> p;
  f.add_abs(1, p);
  f.chmin_cum(true);
 }
 scout << f.min().value() << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "9\n10 5 4 7 9 12 6 2 10\n", "20\n"));
 assert(test(TEST::main, "20\n3 1 4 1 5 9 2 6 5 3 5 8 9 7 9 3 2 3 8 4\n", "41\n"));

 assert(test(TEST_conj::main, "9\n10 5 4 7 9 12 6 2 10\n", "20\n"));
 assert(test(TEST_conj::main, "20\n3 1 4 1 5 9 2 6 5 3 5 8 9 7 9 3 2 3 8 4\n", "41\n"));

 return 0;
}