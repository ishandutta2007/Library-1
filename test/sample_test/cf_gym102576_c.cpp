// competitive-verifier: STANDALONE

// https://codeforces.com/gym/102576/problem/C
// 問題: https://mirror.codeforces.com/gym/102576/attachments/download/10799/200202-tutorial.pdf
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
 int z;
 scin >> z;
 using PLC= PiecewiseLinearConvex<long long>;
 while (z--) {
  int n, d;
  scin >> n >> d;
  long long c[n];
  for (int i= 0; i < n; ++i) cin >> c[i];
  sort(c, c + n);
  PLC f;
  f.add_inf();
  f.add_abs(1, c[0]);
  for (int i= 1; i < n; ++i) {
   f.chmin_cum();
   f.shift(d);
   f.add_abs(1, c[i]);
  }
  scout << (long long)f.min().value() << '\n';
  PLC::reset();
 }
 return 0;
}
}
namespace TEST_conj {
signed main(stringstream& scin, stringstream& scout) {
 int z;
 scin >> z;
 using PLC= PiecewiseLinearConvex<long long>;
 while (z--) {
  int n, d;
  scin >> n >> d;
  long long c[n];
  for (int i= 0; i < n; ++i) cin >> c[i];
  sort(c, c + n);
  PLC f;
  for (int i= 0; i < n; ++i) {
   f.add_inf(true);
   if (i) f.add_linear(d);
   f.add_linear(-c[i]);
   f.chmin_slide_win(-1, 1);
   f.add_linear(c[i]);
  }
  scout << (long long)-f(0).value() << '\n';
  PLC::reset();
 }
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "2\n4 1\n0 0 0 0\n4 10\n1 100 5 10\n", "6\n16\n"));

 assert(test(TEST_conj::main, "2\n4 1\n0 0 0 0\n4 10\n1 100 5 10\n", "6\n16\n"));

 return 0;
}