// competitive-verifier: STANDALONE

// 特に 型推論 auto で受け取っても正しく動作するかテストする。
#include <cassert>
#include "src/LinearAlgebra/Matrix.hpp"

using namespace std;
void mat_int_test() {
 {
  Matrix<int> A= {{1, 2, 3}, {4, 5, 6}}, B= {{1, 2, 3}, {4, 5, 6}};
  assert(A == B);
 }
 {
  Matrix<int> A= {{1, 2, 3}, {4, 5, 6}}, B= {{1, 2, 3}, {4, 5, 7}};
  assert(A != B);
 }
 Matrix<int> A= {{1, 2, 3}, {4, 5, 6}};
 assert(A.width() == 3);
 assert(A.height() == 2);
 {
  auto B= A;
  assert(B[0][0] == 1);
  assert(B[0][1] == 2);
  assert(B[0][2] == 3);
  assert(B[1][0] == 4);
  assert(B[1][1] == 5);
  assert(B[1][2] == 6);
  static_assert(is_same_v<decltype(B), Matrix<int>>);
 }
 {
  auto B= A * 2;
  assert(B[0][0] == 2);
  assert(B[0][1] == 4);
  assert(B[0][2] == 6);
  assert(B[1][0] == 8);
  assert(B[1][1] == 10);
  assert(B[1][2] == 12);
  static_assert(is_same_v<decltype(B), Matrix<int>>);
 }
 {
  auto B= 2 * A;
  assert(B[0][0] == 2);
  assert(B[0][1] == 4);
  assert(B[0][2] == 6);
  assert(B[1][0] == 8);
  assert(B[1][1] == 10);
  assert(B[1][2] == 12);
  static_assert(is_same_v<decltype(B), Matrix<int>>);
 }
 {
  Matrix<int> B= {{2, 3, 4}, {5, 6, 7}};
  auto C= A + B;
  assert(C[0][0] == 3);
  assert(C[0][1] == 5);
  assert(C[0][2] == 7);
  assert(C[1][0] == 9);
  assert(C[1][1] == 11);
  assert(C[1][2] == 13);
  static_assert(is_same_v<decltype(C), Matrix<int>>);
 }
 {
  Vector<int> x= {1, 2, 3};
  auto y= A * x;
  assert(y.size() == 2);
  assert(y[0] == 14);
  assert(y[1] == 32);
  static_assert(is_same_v<decltype(y), Vector<int>>);
 }
 {
  Vector<int> x= {1, 2, 3};
  auto y= A(x);
  assert(y.size() == 2);
  assert(y[0] == 14);
  assert(y[1] == 32);
  static_assert(is_same_v<decltype(y), Vector<int>>);
 }
 {
  Matrix<int> B= {{1, 0}, {0, 1}, {1, 1}};
  auto C= A * B;
  assert(C.width() == 2);
  assert(C.height() == 2);
  assert(C[0][0] == 4);
  assert(C[0][1] == 5);
  assert(C[1][0] == 10);
  assert(C[1][1] == 11);
  static_assert(is_same_v<decltype(C), Matrix<int>>);
 }
}
void mat_bool_test() {
 {
  Matrix<bool> A= {{1, 0, 1}, {0, 1, 0}}, B= {{1, 0, 1}, {0, 1, 0}};
  assert(A == B);
 }
 {
  Matrix<bool> A= {{1, 0, 1}, {0, 1, 0}}, B= {{1, 0, 1}, {0, 1, 1}};
  assert(A != B);
 }
 Matrix<bool> A= {{1, 0, 1}, {0, 1, 0}};
 assert(A.width() == 3);
 assert(A.height() == 2);
 {
  auto B= A;
  assert(B[0][0] == 1);
  assert(B[0][1] == 0);
  assert(B[0][2] == 1);
  assert(B[1][0] == 0);
  assert(B[1][1] == 1);
  assert(B[1][2] == 0);
  static_assert(is_same_v<decltype(B), Matrix<bool>>);
 }
 {
  auto B= A * 1;
  assert(B[0][0] == 1);
  assert(B[0][1] == 0);
  assert(B[0][2] == 1);
  assert(B[1][0] == 0);
  assert(B[1][1] == 1);
  assert(B[1][2] == 0);
  static_assert(is_same_v<decltype(B), Matrix<bool>>);
 }
 {
  auto B= A * 0;
  assert(B[0][0] == 0);
  assert(B[0][1] == 0);
  assert(B[0][2] == 0);
  assert(B[1][0] == 0);
  assert(B[1][1] == 0);
  assert(B[1][2] == 0);
  static_assert(is_same_v<decltype(B), Matrix<bool>>);
 }
 {
  auto B= 1 * A;
  assert(B[0][0] == 1);
  assert(B[0][1] == 0);
  assert(B[0][2] == 1);
  assert(B[1][0] == 0);
  assert(B[1][1] == 1);
  assert(B[1][2] == 0);
  static_assert(is_same_v<decltype(B), Matrix<bool>>);
 }
 {
  auto B= 0 * A;
  assert(B[0][0] == 0);
  assert(B[0][1] == 0);
  assert(B[0][2] == 0);
  assert(B[1][0] == 0);
  assert(B[1][1] == 0);
  assert(B[1][2] == 0);
  static_assert(is_same_v<decltype(B), Matrix<bool>>);
 }
 {
  Matrix<bool> B= {{1, 1, 0}, {0, 1, 1}};
  auto C= A + B;
  assert(C[0][0] == 0);
  assert(C[0][1] == 1);
  assert(C[0][2] == 1);
  assert(C[1][0] == 0);
  assert(C[1][1] == 0);
  assert(C[1][2] == 1);
  static_assert(is_same_v<decltype(C), Matrix<bool>>);
 }
 {
  Vector<bool> x= {1, 1, 0};
  auto y= A * x;
  assert(y.size() == 2);
  assert(y[0] == 1);
  assert(y[1] == 1);
  static_assert(is_same_v<decltype(y), Vector<bool>>);
 }
 {
  Vector<bool> x= {1, 1, 0};
  auto y= A(x);
  assert(y.size() == 2);
  assert(y[0] == 1);
  assert(y[1] == 1);
  static_assert(is_same_v<decltype(y), Vector<bool>>);
 }
 {
  Matrix<bool> B= {{1, 0}, {0, 1}, {1, 1}};
  auto C= A * B;
  assert(C.width() == 2);
  assert(C.height() == 2);
  assert(C[0][0] == 0);
  assert(C[0][1] == 1);
  assert(C[1][0] == 0);
  assert(C[1][1] == 1);
  static_assert(is_same_v<decltype(C), Matrix<bool>>);
 }
}
signed main() {
 mat_int_test();
 mat_bool_test();
 return 0;
}