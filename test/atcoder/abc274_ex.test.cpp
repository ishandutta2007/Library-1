// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc274/tasks/abc274_Ex
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/abc274/tasks/abc274_h
#include <iostream>
#include "src/Math/Nimber.hpp"
#include "src/Misc/Pointwise.hpp"
#include "src/String/RollingHash.hpp"
#include "src/Misc/rng.hpp"
using namespace std;
int main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 Nimber::init();
 using K= Pointwise<Nimber, Nimber>;
 using RH= RollingHash<K, long long>;
 RH::init({rng(), rng()});
 int N, Q;
 cin >> N >> Q;
 vector<long long> A(N);
 for (int i= 0; i < N; i++) cin >> A[i];
 RH rh(A);
 while (Q--) {
  int a, b, c, d, e, f;
  cin >> a >> b >> c >> d >> e >> f;
  a--, c--, e--;
  auto sub1= rh.sub(a, b - a), sub2= rh.sub(c, d - c), sub3= rh.sub(e, f - e);
  int r= min(b - a, f - e);
  int ng= r + 1, ok= 0;
  while (ng - ok > 1) {
   int x= (ng + ok) / 2;
   (sub1.sub(0, x).hash() + sub2.sub(0, x).hash() == sub3.sub(0, x).hash() ? ok : ng)= x;
  }
  if (ok == r) cout << (r < f - e ? "Yes" : "No") << '\n';
  else cout << ((A[a + ok] ^ A[c + ok]) < A[e + ok] ? "Yes" : "No") << '\n';
 }
 return 0;
}