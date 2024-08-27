// competitive-verifier: STANDALONE

// 特に 型推論 auto で受け取っても正しく動作するかテストする。
#include <cassert>
#include "src/LinearAlgebra/Vector.hpp"

using namespace std;
void vec_int_test() {
 {
  Vector<int> x(3);
  assert(x.size() == 3);
  assert(x[0] == 0);
  assert(x[1] == 0);
  assert(x[2] == 0);
 }
 {
  Vector<int> x(3, 2);
  assert(x.size() == 3);
  assert(x[0] == 2);
  assert(x[1] == 2);
  assert(x[2] == 2);
 }
 {
  Vector<int> x= {1, 2, 3}, y= {1, 2, 3};
  assert(x == y);
 }
 {
  Vector<int> x= {1, 2, 3}, y= {1, 2, 4};
  assert(x != y);
 }
 Vector<int> x= {1, 2, 3};
 assert(x.size() == 3);
 {
  auto y= x;
  assert(y[0] == 1);
  assert(y[1] == 2);
  assert(y[2] == 3);
 }
 {
  auto y= x * 2;
  assert(y[0] == 2);
  assert(y[1] == 4);
  assert(y[2] == 6);
  static_assert(is_same_v<decltype(y), Vector<int>>);
 }
 {
  Vector<int> y= {4, 5, 6};
  auto z= x + y;
  assert(z[0] == 5);
  assert(z[1] == 7);
  assert(z[2] == 9);
  static_assert(is_same_v<decltype(z), Vector<int>>);
 }
 {
  x[0]= 2;
  assert(x[0] == 2);
  assert(x[1] == 2);
  assert(x[2] == 3);
 }
 {
  Vector<int> y= {4, 5, 6};
  assert(dot(x, y) == 32);
 }
}
void vec_bool_test() {
 {
  Vector<bool> x(3);
  assert(x.size() == 3);
  assert(x[0] == 0);
  assert(x[1] == 0);
  assert(x[2] == 0);
 }
 {
  Vector<bool> x(3, 1);
  assert(x.size() == 3);
  assert(x[0] == 1);
  assert(x[1] == 1);
  assert(x[2] == 1);
 }
 {
  Vector<bool> x= {1, 0, 1}, y= {1, 0, 1};
  assert(x == y);
 }
 {
  Vector<bool> x= {1, 0, 1}, y= {1, 0, 0};
  assert(x != y);
 }
 {
  Vector<bool> x(127), y(127);
  x[126]= 1;
  assert(x != y);
 }
 {
  Vector<bool> x(128), y(128);
  x[127]= 1;
  assert(x != y);
 }
 {
  Vector<bool> x(129), y(129);
  x[128]= 1;
  assert(x != y);
 }
 Vector<bool> x= {1, 0, 1};
 assert(x.size() == 3);
 {
  auto y= x;
  assert(y[0] == 1);
  assert(y[1] == 0);
  assert(y[2] == 1);
 }
 {
  auto y= x * 1;
  assert(y[0] == 1);
  assert(y[1] == 0);
  assert(y[2] == 1);
  static_assert(is_same_v<decltype(y), Vector<bool>>);
 }
 {
  auto y= x * 0;
  assert(y[0] == 0);
  assert(y[1] == 0);
  assert(y[2] == 0);
  static_assert(is_same_v<decltype(y), Vector<bool>>);
 }
 {
  Vector<bool> y= {0, 1, 1};
  auto z= x + y;
  assert(z[0] == 1);
  assert(z[1] == 1);
  assert(z[2] == 0);
  static_assert(is_same_v<decltype(z), Vector<bool>>);
 }
 {
  x[0]= 0;
  assert(x[0] == 0);
  assert(x[1] == 0);
  assert(x[2] == 1);
 }
 {
  Vector<bool> y= {0, 1, 1};
  assert(dot(x, y) == 1);
 }
}
signed main() {
 vec_int_test();
 vec_bool_test();
 return 0;
}