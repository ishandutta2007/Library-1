// competitive-verifier: PROBLEM https://www.hackerrank.com/challenges/morgan-and-a-string
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <string>
#include "mylib/algebra/ModInt.hpp"
#include "mylib/misc/Pointwise.hpp"
#include "mylib/string/RollingHash.hpp"
#include "mylib/misc/rng.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 using K= Pointwise<Mint, Mint>;
 using RH= RollingHash<K>;
 RH::init({rng(), rng()});
 int T;
 cin >> T;
 while(T--) {
  string a, b;
  cin >> a >> b;
  int n= a.length(), m= b.length();
  a+= 'Z' + 1, b+= 'Z' + 1;
  RH rha(a), rhb(b);
  for(int i= 0, j= 0; i < n || j < m;) {
   if(rha.sub(i) < rhb.sub(j)) cout << a[i++];
   else cout << b[j++];
  }
  cout << '\n';
 }
 return 0;
}
