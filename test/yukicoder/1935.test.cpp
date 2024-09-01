// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1935
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <array>
#include "src/Misc/Period.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Dat= array<int, 4>;
 Dat V;
 long long N;
 cin >> V[0] >> V[1] >> V[2] >> V[3] >> N;
 auto f= [&](int &a, int &b, int v) {
  int x= min(a, v - b);
  a-= x, b+= x;
 };
 auto g= [&](Dat a) {
  f(a[0], a[1], V[1]);
  f(a[1], a[2], V[2]);
  f(a[2], a[3], V[3]);
  f(a[3], a[0], V[0]);
  return a;
 };
 Dat init= {V[0], 0, 0, 0};
 Period<Dat> p(g, {init});
 auto [q, r]= div(N, 4LL);
 auto v= p.jump(init, q);
 for (int i= 0; i < r; ++i) f(v[i], v[i + 1], V[i + 1]);
 cout << v[0] << ' ' << v[1] << ' ' << v[2] << ' ' << v[3] << '\n';
 return 0;
}