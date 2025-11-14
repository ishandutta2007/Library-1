// competitive-verifier: PROBLEM https://loj.ac/p/6686
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/ArrayOnDivisors.hpp"
#include "src/NumberTheory/sum_on_primes.hpp"
using namespace std;
template <class Tp> void read(Tp &x) {
 static char ch;
 static bool neg;
 for (ch= neg= 0; ch < '0' || ch > '9'; neg|= (ch == '-'), ch= getchar());
 for (x= 0; ch >= '0' && ch <= '9'; (x*= 10)+= (ch ^ 48), ch= getchar());
 neg && (x= -x);
}
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 using u128= __uint128_t;
 using u64= uint64_t;
 u128 n= 0;
 read(n);
 u64 l= 0, h= 1e10 + 10;
 while (h - l > 1) {
  u64 x= (h + l) / 2;
  u128 t= u128(x) * x * x;
  if (t <= n) l= x;
  else h= x;
 }
 u64 r= l - 1;
 u128 m= u128(l) * l * l;
 Mint ans= 0;
 ArrayOnDivisors<u64, u64> totient(l);
 totient.set_totient();
 for (auto [d, phi]: totient) ans+= Mint(n / d - (m - 1) / d) * phi;
 auto Ps= sums_of_powers_on_primes<Mint>(r, 2);
 auto f= [&](Mint p, int e) { return p.pow(e - 1) * (p * (e + 1) - e); };
 auto g= [&](Mint p, int e) { return p.pow(e + e - 1) * (p * (e + 1) - e); };
 ans+= multiplicative_sum<Mint>(2 * Ps[1] - Ps[0], f) * 3;
 ans+= multiplicative_sum<Mint>(2 * Ps[2] - Ps[1], g) * 3;
 ans+= Mint(r) * (r + 1) / 2;
 cout << ans << '\n';
 return 0;
}