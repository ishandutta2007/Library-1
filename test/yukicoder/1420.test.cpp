// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1420
// competitive-verifier: TLE 2
// competitive-verifier: MLE 256
#include <iostream>
#include "mylib/data_structure/UnionFind_Potentialized.hpp"
#include "mylib/algebra/Algebra.hpp"
using namespace std;
struct Xor {
 using T= unsigned;
 static constexpr T o= 0;
 static T add(const T& a, const T& b) { return a ^ b; }
 static T neg(const T& a) { return a; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using G= Algebra<Xor>;
 int N, M;
 cin >> N >> M;
 UnionFind_Potentialized<G> uf(N);
 bool ok= true;
 for(int i= 0; i < M; ++i) {
  int a, b;
  unsigned y;
  cin >> a >> b >> y;
  --a, --b;
  if(!uf.unite(a, b, G(y))) ok= false;
 }
 if(!ok) {
  cout << -1 << '\n';
 } else {
  for(int i= 0; i < N; ++i) cout << uf.potential(i).x << '\n';
 }
 return 0;
}
