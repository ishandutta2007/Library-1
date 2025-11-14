// competitive-verifier: STANDALONE

// https://oj.uz/problem/view/NOI18_safety
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
 int N, H;
 scin >> N >> H;
 PiecewiseLinearConvex<long long> f;
 for (int i= 0; i < N; ++i) {
  int S;
  scin >> S;
  f.chmin_slide_win(-H, H);
  f.add_abs(1, S);
 }
 scout << (long long)f.min().value() << '\n';
 return 0;
}
}
namespace TEST_conj {
signed main(stringstream& scin, stringstream& scout) {
 int N, H;
 scin >> N >> H;
 PiecewiseLinearConvex<long long> f;
 f.add_inf(), f.add_inf(true);
 for (int i= 0; i < N; ++i) {
  int S;
  scin >> S;
  f.add_abs(H, 0);
  f.add_linear(-S);
  f.chmin_slide_win(-1, 1);
  f.add_linear(S);
 }
 scout << (long long)-f(0).value() << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "6 1\n2 10 0 2 4 3\n", "10\n"));
 assert(test(TEST::main, "6 3\n2 10 2 6 4 3\n", "6\n"));
 assert(test(TEST::main, "4 1\n1 4 1 4\n", "4\n"));
 assert(test(TEST::main, "10 1\n10 9 8 7 6 5 4 3 2 1\n", "0\n"));
 assert(test(TEST::main, "3 0\n1 1 3\n", "2\n"));

 assert(test(TEST_conj::main, "6 1\n2 10 0 2 4 3\n", "10\n"));
 assert(test(TEST_conj::main, "6 3\n2 10 2 6 4 3\n", "6\n"));
 assert(test(TEST_conj::main, "4 1\n1 4 1 4\n", "4\n"));
 assert(test(TEST_conj::main, "10 1\n10 9 8 7 6 5 4 3 2 1\n", "0\n"));
 assert(test(TEST_conj::main, "3 0\n1 1 3\n", "2\n"));

 return 0;
}