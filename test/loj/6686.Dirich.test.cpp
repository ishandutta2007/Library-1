// competitive-verifier: PROBLEM https://loj.ac/p/6686
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/ArrayOnDivisors.hpp"
#include "src/NumberTheory/DirichletSeries.hpp"
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
 auto zeta= get_1<Mint>(r), id= get_Id<Mint>(r), id2= get_Id2<Mint>(r);
 ans+= (id2.square() / id).sum() * 3;
 ans+= (id.square() / zeta).sum() * 3;
 ans+= id.sum();
 cout << ans << '\n';
 return 0;
}