// competitive-verifier: PROBLEM https://loj.ac/p/572
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <iostream>
#include "src/NumberTheory/CumSumQuotient.hpp"
#include "src/NumberTheory/sum_on_primes.hpp"
#include "src/NumberTheory/DirichletSeries.hpp"
#include "src/NumberTheory/enumerate_quotients.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 unsigned N, k;
 cin >> N >> k;
 auto pi= sums_of_powers_on_primes<unsigned>(N, 0)[0];
 auto eq= enumerate_quotients(N);
 auto primes= enumerate_primes(sqrt(N));
 CumSumQuotient<unsigned> f(N);
 for (int i= primes.size(); i--;) {
  unsigned p= primes[i], p2= p * p, pk= 1;
  for (unsigned e= k, b= p;; b*= b) {
   if (e & 1) pk*= b;
   if (!(e>>= 1)) break;
  }
  for (int j= eq.size(); j--;) {
   unsigned n= get<0>(eq[j]);
   if (n < p2) break;
   auto &cur= f[n];
   for (unsigned pw= p; uint64_t(pw) * p <= n; pw*= p) {
    unsigned m= n / pw;
    cur+= pk * (pi(m) - i) + f[m];
   }
  }
 }
 for (auto [q, l, r]: eq) f[q]+= pi(q);
 DirichletSeries<unsigned> F(N, f);
 cout << (get_phi<unsigned>(N) * F).sum() * 2 - F.sum() << '\n';
 return 0;
}