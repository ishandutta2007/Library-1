// competitive-verifier: STANDALONE

// https://codeforces.com/contest/1209/problem/H
#include <sstream>
#include <string>
#include <cassert>
#include <iomanip>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 double x= stod(scout.str());
 double ex= stod(expected);
 double err= abs(x - ex);
 return err <= 1e-9 && err / abs(ex) <= 1e-9;
}
namespace TEST {
signed main(stringstream& scin, stringstream& scout) {
 int n, L;
 scin >> n >> L;
 PiecewiseLinearConvex<long double> f;
 f.add_inf(), f.add_inf(true);
 auto no_belt= [&](int y, int x) {
  if (y == x) return;
  f.add_linear(-1);
  f.chmin_cum();
  f.shift(-double(x - y) / 2);
  f.add_linear(1);
  f.add_const(x - y);
  f.add_inf();
 };
 int x, y= 0;
 for (int i= 0; i < n; ++i) {
  scin >> x;
  no_belt(y, x);
  double s;
  scin >> y >> s;
  f.add_linear(-1. / (1 + s));
  f.chmin_slide_win(-double(y - x) / (2 + s), double(y - x) / s);
  f.add_linear(1. / (1 + s));
  f.add_const(double(y - x) / (1 + s));
  f.add_inf();
 }
 no_belt(y, L);
 scout << fixed << setprecision(12) << f.min().value() << '\n';
 return 0;
}
}
namespace TEST_conj {
signed main(stringstream& scin, stringstream& scout) {
 int n, L;
 scin >> n >> L;
 PiecewiseLinearConvex<long double> f;
 auto no_belt= [&](int y, int x) {
  if (y == x) return;
  f.add_inf(true, 1);
  f.add_linear(-double(x - y) / 2);
  f.add_const(-double(x - y) / 2);
  f.chmin_cum(true);
 };
 int x, y= 0;
 for (int i= 0; i < n; ++i) {
  scin >> x;
  no_belt(y, x);
  double s;
  scin >> y >> s;
  f.add_max(-double(y - x) / (2 + s), double(y - x) / s, 1. / (1 + s));
  f.add_const(-double(y - x) / (1 + s));
  f.chmin_cum(true);
 }
 no_belt(y, L);
 scout << fixed << setprecision(12) << -f(0).value() << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "1 5\n0 2 2.0\n", "3.000000000000\n"));
 assert(test(TEST::main, "1 5\n2 4 0.91\n", "3.808900523560\n"));
 assert(test(TEST::main, "3 1000\n0 990 1.777777\n995 996 1.123456789\n996 1000 2.0\n", "361.568848429553\n"));

 assert(test(TEST_conj::main, "1 5\n0 2 2.0\n", "3.000000000000\n"));
 assert(test(TEST_conj::main, "1 5\n2 4 0.91\n", "3.808900523560\n"));
 assert(test(TEST_conj::main, "3 1000\n0 990 1.777777\n995 996 1.123456789\n996 1000 2.0\n", "361.568848429553\n"));
 return 0;
}