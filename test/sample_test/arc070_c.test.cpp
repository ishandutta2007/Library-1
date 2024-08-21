// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/arc070/tasks/arc070_c
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
 long long pw;
 for (int i= 0; i < N; ++i) {
  long long l, r;
  scin >> l >> r;
  long long w= r - l;
  if (i) f.chmin_slide_win(-w, pw);
  f.add_abs(1, l);
  pw= w;
 }
 scout << (long long)f.min().value() << '\n';
 return 0;
}
}
namespace TEST_conj {
signed main(stringstream& scin, stringstream& scout) {
 int N;
 scin >> N;
 PiecewiseLinearConvex<long long> f;
 f.add_inf(), f.add_inf(true);
 long long pw;
 for (int i= 0; i < N; ++i) {
  long long l, r;
  scin >> l >> r;
  long long w= r - l;
  if (i) f.add_max(-w, pw, 0);
  f.add_linear(-l);
  f.chmin_slide_win(-1, 1);
  f.add_linear(l);
  pw= w;
 }
 scout << (long long)-f(0).value() << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "3\n1 3\n5 7\n1 3\n", "2\n"));
 assert(test(TEST::main, "3\n2 5\n4 6\n1 4\n", "0\n"));
 assert(test(TEST::main, "5\n999999999 1000000000\n1 2\n314 315\n500000 500001\n999999999 1000000000\n", "1999999680\n"));
 assert(test(TEST::main, "5\n123456 789012\n123 456\n12 345678901\n123456 789012\n1 23\n", "246433\n"));
 assert(test(TEST::main, "1\n1 400\n", "0\n"));

 assert(test(TEST_conj::main, "3\n1 3\n5 7\n1 3\n", "2\n"));
 assert(test(TEST_conj::main, "3\n2 5\n4 6\n1 4\n", "0\n"));
 assert(test(TEST_conj::main, "5\n999999999 1000000000\n1 2\n314 315\n500000 500001\n999999999 1000000000\n", "1999999680\n"));
 assert(test(TEST_conj::main, "5\n123456 789012\n123 456\n12 345678901\n123456 789012\n1 23\n", "246433\n"));
 assert(test(TEST_conj::main, "1\n1 400\n", "0\n"));

 return 0;
}