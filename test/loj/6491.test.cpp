// competitive-verifier: PROBLEM https://loj.ac/p/6491
// competitive-verifier: TLE 1
// competitive-verifier: MLE 128

#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/DirichletSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 uint64_t n, m;
 cin >> n >> m;
 auto gsum= [&](uint64_t x) {
  uint64_t ret= 1;
  for (uint64_t e= n;; x*= x) {
   if (e & 1) ret*= x;
   if (!(e>>= 1)) break;
  }
  return ret;
 };
 DirichletSeries<uint64_t> g(m, gsum);
 cout << (get_phi<uint64_t>(m) * g).sum() << '\n';
 return 0;
}