// competitive-verifier: PROBLEM https://www.hackerrank.com/contests/happy-query-contest/challenges/grid-xor-query
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/DataStructure/KDTree.hpp"

// 2次元xor
using namespace std;
struct RxorQ {
 using T= int;
 static T ti() { return 0; }
 static T op(T l, T r) { return l ^ r; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using KDT= KDTree<int, 2, RxorQ>;
 int N;
 cin >> N;
 vector<array<int, 3>> vec(N);
 for (int i= 0; i < N; i++) {
  int x, y, v;
  cin >> x >> y >> v;
  vec[i]= {x, y, v};
 }
 KDT kdt(vec);
 int Q;
 cin >> Q;
 while (Q--) {
  int a, b, c, d;
  cin >> a >> b >> c >> d;
  cout << kdt.prod_cuboid(a, b, c, d) << '\n';
 }
 return 0;
}