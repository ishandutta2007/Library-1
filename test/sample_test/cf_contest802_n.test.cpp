// competitive-verifier: STANDALONE

// https://codeforces.com/contest/802/problem/N

// より制約がきつい問題 (https://codeforces.com/contest/802/problem/O) もあるが，そっちはTLE
#include <sstream>
#include <string>
#include <cassert>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
#include "src/Optimization/fibonacci_search.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
signed main(stringstream& scin, stringstream& scout) {
 using PLC= PiecewiseLinearConvex<int>;
 int n, k;
 scin >> n >> k;
 vector<int> a(n), b(n);
 for (int i= 0; i < n; ++i) scin >> a[i];
 for (int i= 0; i < n; ++i) scin >> b[i];
 auto lagrange= [&](int p) {
  PLC f;
  f.add_inf(), f.add_inf(true);
  for (int i= 0; i < n; ++i) {
   if (a[i] < p - b[i]) {
    f.add_linear(b[i] - p);
    f.chmin_slide_win(0, 1);
    f.add_linear(p - b[i] - a[i]);
    f.chmin_slide_win(-1, 0);
    f.add_linear(a[i]);
    f.add_const(a[i] + b[i] - p);
   } else {
    f.add_linear(-a[i]);
    f.chmin_slide_win(0, 1);
    f.add_linear(a[i] + b[i] - p);
    f.chmin_slide_win(-1, 0);
    f.add_linear(p - b[i]);
   }
   f.add_inf();
  }
  long long ret= f.min().value() + (long long)p * k;
  PLC::reset();
  return ret;
 };
 scout << fibonacci_search<MAXIMIZE>(lagrange, 0LL, 2e9 + 10).second << '\n';
 return 0;
}
}
namespace TEST_conj {
signed main(stringstream& scin, stringstream& scout) {
 using PLC= PiecewiseLinearConvex<int>;
 int n, k;
 scin >> n >> k;
 vector<int> a(n), b(n);
 for (int i= 0; i < n; ++i) scin >> a[i];
 for (int i= 0; i < n; ++i) scin >> b[i];
 auto lagrange= [&](int p) {
  PLC f;
  for (int i= 0; i < n; ++i) {
   if (a[i] < p - b[i]) {
    f.add_ramp(1, p - b[i]);
    f.add_ramp(-1, a[i]);
    f.add_const(p - a[i] - b[i]);
   } else {
    f.add_ramp(1, a[i]);
    f.add_ramp(-1, p - b[i]);
   }
   f.chmin_cum(true);
  }
  long long ret= -f(0).value() + (long long)p * k;
  PLC::reset();
  return ret;
 };
 scout << fibonacci_search<MAXIMIZE>(lagrange, 0, 2e9 + 10).second << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "8 4\n3 8 7 9 9 4 6 8\n2 5 9 4 3 8 9 1\n", "32\n"));

 assert(test(TEST_conj::main, "8 4\n3 8 7 9 9 4 6 8\n2 5 9 4 3 8 9 1\n", "32\n"));
 return 0;
}