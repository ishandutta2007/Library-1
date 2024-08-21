// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/dwango2016-prelims/tasks/dwango2016qual_e
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
 int N, L;
 scin >> N >> L;
 PiecewiseLinearConvex<int> f;
 for (int prev= -1, t, P; N--; prev= t) {
  scin >> t >> P;
  if (prev != t) f.chmin_cum();
  f.add_abs(1, P);
 }
 scout << f.min().value() << '\n';
 return 0;
}
}
namespace TEST_conj {
signed main(stringstream& scin, stringstream& scout) {
 int N, L;
 scin >> N >> L;
 PiecewiseLinearConvex<int> f;
 f.add_inf(), f.add_inf(true);
 for (int prev= -1, t, P; N--; prev= t) {
  scin >> t >> P;
  if (prev != t) f.add_inf();
  f.add_linear(P);
  f.chmin_slide_win(-1, 1);
  f.add_linear(-P);
 }
 scout << -f(0).value() << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "5 10\n1 2\n1 4\n3 8\n4 7\n5 1\n", "9\n"));
 assert(test(TEST::main, "4 10\n1 4\n1 4\n2 1\n3 9\n", "3\n"));
 assert(test(TEST::main, "10 20\n2 15\n3 4\n3 14\n4 11\n6 0\n7 7\n8 8\n8 8\n8 12\n9 10\n", "33\n"));

 assert(test(TEST_conj::main, "5 10\n1 2\n1 4\n3 8\n4 7\n5 1\n", "9\n"));
 assert(test(TEST_conj::main, "4 10\n1 4\n1 4\n2 1\n3 9\n", "3\n"));
 assert(test(TEST_conj::main, "10 20\n2 15\n3 4\n3 14\n4 11\n6 0\n7 7\n8 8\n8 8\n8 12\n9 10\n", "33\n"));
 return 0;
}