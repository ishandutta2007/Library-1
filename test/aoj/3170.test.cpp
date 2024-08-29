// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/3170
// competitive-verifier: TLE 2
#include <iostream>
#include <vector>
#include "src/DataStructure/SortedPerBucket.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 vector<long long> a(N);
 for (int i= 0; i < N; i++) cin >> a[i];
 SortedPerBucket<long long, 100> sqrtdc(a);
 while (Q--) {
  int op, l, r;
  long long x;
  cin >> op >> l >> r >> x;
  l--;
  if (op == 4) {
   long long y;
   cin >> y;
   cout << sqrtdc.count(l, r, x, y + 1) << '\n';
  } else {
   if (op == 1) sqrtdc.chmin(l, r, x);
   if (op == 2) sqrtdc.chmax(l, r, x);
   if (op == 3) sqrtdc.add(l, r, x);
  }
 }
 return 0;
}