#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/14/ALDS1_14_B"
#include <iostream>
#include <string>
#include "src/Math/ModInt.hpp"
#include "src/Math/CartesianProduct.hpp"
#include "src/String/RollingHash.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<(1ll << 61) - 1>;
 using K= CartesianProduct<Mint, Mint>;
 using RH= RollingHash<K>;
 K base= {get_rand(2, (1ll << 61) - 2), get_rand(2, (1ll << 61) - 2)};
 RH::set_base(base);
 string T, P;
 cin >> T >> P;
 RH rt(T), rp(P);
 int N= P.length(), M= T.length();
 auto hash= rp.get_hash();
 for (int i= 0; i + N <= M; i++)
  if (rt.get_hash(i, i + N) == hash) cout << i << "\n";
 return 0;
}