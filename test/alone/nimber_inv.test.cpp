// competitive-verifier: STANDALONE
#include <iostream>
#include <random>
#include <cassert>
#include "src/Math/Nimber.hpp"
using namespace std;
void test(int X) {
 mt19937 mt(X);
 uniform_int_distribution<uint64_t> rng(1, uint64_t(-1));
 static constexpr int N= 100000;
 static Nimber a[N], b[N], c[N];
 for (int i= 0; i < N; i++) a[i]= rng(mt), b[i]= rng(mt), c[i]= a[i] * b[i];
 for (int i= 0; i < N; i++) {
  Nimber ans= c[i] / b[i];
  assert(ans == a[i]);
  assert(ans.val() == a[i].val());
 }
}
signed main() {
 Nimber::init();
 test(0), test(1);
 return 0;
}