// competitive-verifier: STANDALONE
#include <iostream>
#include <random>
#include <cassert>
#include "src/Math/Nimber.hpp"
using namespace std;
void test(int X) {
 mt19937 mt(X);
 uniform_int_distribution<uint64_t> rng(0, uint64_t(-1));
 static constexpr int N= 1000;
 static Nimber a[N], b[N];
 static uint64_t x[N];
 for (int i= 0; i < N; i++) a[i]= rng(mt), x[i]= rng(mt), b[i]= a[i].pow(x[i]);
 for (int i= 0; i < N; i++) {
  uint64_t ans= a[i].log(b[i]);
  assert(a[i].pow(ans) == b[i]);
 }
}
signed main() {
 Nimber::init();
 test(0), test(1);
 return 0;
}