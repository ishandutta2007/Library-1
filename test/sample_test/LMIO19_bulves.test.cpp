// competitive-verifier: STANDALONE

// https://oj.uz/problem/view/LMIO19_bulves
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
 PiecewiseLinearConvex<long long> f;
 f.add_inf(true);
 for (int i= 0; i < N; ++i) {
  int a, b;
  scin >> a >> b;
  f.chmin_cum(true);
  f.shift(a - b);
  f.add_abs(1, 0);
 }
 scout << (long long)f(0).value() << '\n';
 return 0;
}
}
namespace TEST_conj {
signed main(stringstream& scin, stringstream& scout) {
 int N;
 scin >> N;
 PiecewiseLinearConvex<long long> f;
 for (int i= 0; i < N; ++i) {
  int a, b;
  scin >> a >> b;
  f.add_inf();
  f.add_linear(a - b);
  f.chmin_slide_win(-1, 1);
 }
 scout << (long long)-f.min().value() << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "6\n1 2\n0 0\n2 0\n0 0\n0 0\n0 1\n", "5\n"));
 assert(test(TEST::main, "7\n2 0\n2 0\n2 0\n0 5\n2 0\n2 0\n2 0\n", "6\n"));

 assert(test(TEST_conj::main, "6\n1 2\n0 0\n2 0\n0 0\n0 0\n0 1\n", "5\n"));
 assert(test(TEST_conj::main, "7\n2 0\n2 0\n2 0\n0 5\n2 0\n2 0\n2 0\n", "6\n"));

 return 0;
}