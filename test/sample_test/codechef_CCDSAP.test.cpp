// competitive-verifier: STANDALONE

// https://www.codechef.com/problems/CCDSAP
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
 int T;
 scin >> T;
 using PLC= PiecewiseLinearConvex<long long>;
 while (T--) {
  int N;
  scin >> N;
  string S;
  scin >> S;
  vector<int> L;
  for (int i= 0; i < N; ++i) {
   if (S[i] == '0') continue;
   L.push_back(i);
  }
  if (L.size() == 0) {
   scout << 0 << '\n';
   continue;
  }
  if ((int)L.size() > (N + 1) / 2) {
   scout << "impossible" << '\n';
   continue;
  }
  int n= L.size();
  PLC f;
  f.add_inf();
  f.add_abs(1, L[0]);
  for (int i= 1; i < n; ++i) {
   f.chmin_cum();
   f.shift(2);
   f.add_abs(1, L[i]);
  }
  f.add_inf(true, N - 1);
  scout << (long long)f.min().value() << '\n';
  PLC::reset();
 }
 return 0;
}
}
namespace TEST_conj {
signed main(stringstream& scin, stringstream& scout) {
 int T;
 scin >> T;
 using PLC= PiecewiseLinearConvex<long long>;
 while (T--) {
  int N;
  scin >> N;
  string S;
  scin >> S;
  vector<int> L;
  for (int i= 0; i < N; ++i) {
   if (S[i] == '0') continue;
   L.push_back(i);
  }
  if (L.size() == 0) {
   scout << 0 << '\n';
   continue;
  }
  if ((int)L.size() > (N + 1) / 2) {
   scout << "impossible" << '\n';
   continue;
  }
  int n= L.size();
  PLC f;
  f.add_inf(true);
  f.add_linear(-L[0]);
  f.chmin_slide_win(-1, 1);
  f.add_linear(L[0]);
  for (int i= 1; i < n; ++i) {
   f.add_inf(true);
   f.add_linear(2);
   f.add_linear(-L[i]);
   f.chmin_slide_win(-1, 1);
   f.add_linear(L[i]);
  }
  f.add_linear(-(N - 1));
  f.chmin_cum();
  f.add_linear(N - 1);
  scout << (long long)-f(0).value() << '\n';
  PLC::reset();
 }
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "4\n3\n011\n5\n10111\n1\n1\n6\n000111\n", "1\nimpossible\n0\n3\n"));

 assert(test(TEST_conj::main, "4\n3\n011\n5\n10111\n1\n1\n6\n000111\n", "1\nimpossible\n0\n3\n"));

 return 0;
}