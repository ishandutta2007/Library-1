// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/abc009/tasks/abc009_4
// bitwise xor and
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include "src/Math/Algebra.hpp"
#include "src/LinearAlgebra/Matrix.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
struct Rig {
 using T= unsigned;
 static constexpr T o= 0, i= -1;
 static T add(T a, T b) { return a ^ b; }
 static T mul(T a, T b) { return a & b; }
};
signed main(stringstream& scin, stringstream& scout) {
 using R= Algebra<Rig>;
 Vector<R> A(100);
 Matrix<R> C(100, 100);
 int K, M;
 scin >> K >> M, M--;
 for (int i= 0; i < K; i++) scin >> A[K - i - 1];
 for (int i= 0; i < K; i++) scin >> C[0][i];
 for (int i= 1; i < K; i++) C[i][i - 1]= R(true);
 scout << (M < K ? A[K - M - 1] : (C.pow(M - (K - 1)) * A)[0]) << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "3 5\n10 20 30\n7 19 13\n", "16\n"));
 assert(test(TEST::main,
             "5 100\n"
             "2345678901 1001001001 3333333333 3141592653 1234567890\n"
             "2147483648 2147483647 4294967295 4294967294 3434343434\n",
             "1067078691\n"));
 assert(test(TEST::main,
             "30 999999999\n"
             "11627 5078 8394 6412 10346 3086 3933 668 9879 11739 4501 6108 12336 8771 2768 2438 2153 7047 5476 313 1264 369 12070 10743 10663 747 370 4671 5235 3439\n"
             "114 3613 3271 5032 11241 6961 3628 150 12191 2396 7638 3046 11594 8162 11136 786 9878 2356 11660 1070 3649 10882 9746 1415 3307 7077 9319 9981 3437 544\n",
             "2148\n"));
 return 0;
}