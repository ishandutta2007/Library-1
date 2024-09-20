// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/ddcc2019-final/tasks/ddcc2019_final_d
#include <sstream>
#include <string>
#include <cassert>
#include <vector>
#include "src/LinearAlgebra/Matrix.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
signed main(stringstream& scin, stringstream& scout) {
 using Vec= Vector<unsigned>;
 using Mat= Matrix<unsigned>;
 string S;
 scin >> S;
 int N= S.length();
 vector<Vec> a(N + 1, Vec(6)), b(N + 1, Vec(6));
 Mat A= Mat::identity(6), B= Mat::identity(6);
 a[0]= {0, 0, 0, 0, 0, 1}, b[0]= {1, 0, 0, 0, 0, 0};
 for (int i= 0; i < N; ++i) {
  if (S[i] == 'D') {
   for (int j= 0; j < 1; ++j) A[1][j]+= A[0][j];
   for (int j= 1; j < 6; ++j) B[j][0]-= B[j][1];
  } else if (S[i] == 'I') {
   for (int j= 0; j < 2; ++j) A[2][j]+= A[1][j];
   for (int j= 2; j < 6; ++j) B[j][1]-= B[j][2];
  } else if (S[i] == 'S') {
   for (int j= 0; j < 3; ++j) A[3][j]+= A[2][j];
   for (int j= 3; j < 6; ++j) B[j][2]-= B[j][3];
  } else if (S[i] == 'C') {
   for (int j= 0; j < 4; ++j) A[4][j]+= A[3][j];
   for (int j= 4; j < 6; ++j) B[j][3]-= B[j][4];
  } else {
   for (int j= 0; j < 5; ++j) A[5][j]+= A[4][j];
   for (int j= 5; j < 6; ++j) B[j][4]-= B[j][5];
  }
  for (int j= 0; j < 6; ++j) a[i + 1][j]= A[5][j];
  for (int j= 0; j < 6; ++j) b[i + 1][j]= B[j][0];
 }
 int Q;
 scin >> Q;
 while (Q--) {
  int L, R;
  scin >> L >> R;
  scout << dot(a[R], b[L - 1]) << '\n';
 }
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "DDDDDDISCOOOOOO\n7\n6 10\n5 11\n4 12\n3 13\n2 14\n1 15\n1 8\n", "1\n4\n9\n16\n25\n36\n0\n"));
 assert(test(TEST::main, "DDDIIISSSCCCOOO\n12\n1 12\n1 13\n1 14\n1 15\n2 12\n2 13\n2 14\n2 15\n3 13\n3 14\n3 15\n4 15\n", "0\n81\n162\n243\n0\n54\n108\n162\n27\n54\n81\n0\n"));
 return 0;
}