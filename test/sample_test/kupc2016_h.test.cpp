// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/kupc2016/tasks/kupc2016_h
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
 long long A[N], B[N];
 for (int i= 0; i < N; ++i) scin >> A[i];
 for (int i= 0; i < N; ++i) scin >> B[i];
 PiecewiseLinearConvex<long long> f;
 f.add_inf();
 for (int i= 0; i < N; ++i) {
  f.chmin_cum();
  f.shift(B[i] - A[i]);
  if (i < N - 1) f.add_abs(1, 0);
 }
 scout << (long long)f(0) << '\n';
 return 0;
}
}
namespace TEST_conj {
signed main(stringstream& scin, stringstream& scout) {
 int N;
 scin >> N;
 long long A[N], B[N];
 for (int i= 0; i < N; ++i) scin >> A[i];
 for (int i= 0; i < N; ++i) scin >> B[i];
 PiecewiseLinearConvex<long long> f;
 for (int i= 0; i < N; ++i) {
  f.add_inf();
  f.add_linear(-B[i] + A[i]);
  if (i < N - 1) f.chmin_slide_win(-1, 1);
 }
 scout << (long long)-f.min() << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "2\n1 5\n3 1\n", "2\n"));
 assert(test(TEST::main, "5\n1 2 3 4 5\n3 3 1 1 1\n", "6\n"));
 assert(test(TEST::main, "27\n46 3 4 2 10 2 5 2 6 7 20 13 9 49 3 8 4 3 19 9 3 5 4 13 9 5 7\n10 2 5 6 2 6 3 2 2 5 3 11 13 2 2 7 7 3 9 5 13 4 17 2 2 2 4\n", "48\n"));
 assert(test(TEST::main,
             "18\n"
             "3878348 423911 8031742 1035156 24256 10344593 19379 3867285 4481365 1475384 1959412 1383457 164869 4633165 6674637 9732852 10459147 2810788\n"
             "1236501 770807 4003004 131688 1965412 266841 3980782 565060 816313 192940 541896 250801 217586 3806049 1220252 1161079 31168 2008961\n",
             "6302172\n"));
 assert(test(TEST::main, "2\n1 99999999999\n1234567891 1\n", "1234567890\n"));

 assert(test(TEST_conj::main, "2\n1 5\n3 1\n", "2\n"));
 assert(test(TEST_conj::main, "5\n1 2 3 4 5\n3 3 1 1 1\n", "6\n"));
 assert(test(TEST_conj::main, "27\n46 3 4 2 10 2 5 2 6 7 20 13 9 49 3 8 4 3 19 9 3 5 4 13 9 5 7\n10 2 5 6 2 6 3 2 2 5 3 11 13 2 2 7 7 3 9 5 13 4 17 2 2 2 4\n", "48\n"));
 assert(test(TEST_conj::main,
             "18\n"
             "3878348 423911 8031742 1035156 24256 10344593 19379 3867285 4481365 1475384 1959412 1383457 164869 4633165 6674637 9732852 10459147 2810788\n"
             "1236501 770807 4003004 131688 1965412 266841 3980782 565060 816313 192940 541896 250801 217586 3806049 1220252 1161079 31168 2008961\n",
             "6302172\n"));
 assert(test(TEST_conj::main, "2\n1 99999999999\n1234567891 1\n", "1234567890\n"));
 return 0;
}