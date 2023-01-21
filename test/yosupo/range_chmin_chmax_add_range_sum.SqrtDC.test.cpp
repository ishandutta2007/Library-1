#define PROBLEM "https://judge.yosupo.jp/problem/range_chmin_chmax_add_range_sum"
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
 SortedPerBucket<long long> sqrtdc(a);
 while (Q--) {
  int op, l, r;
  cin >> op >> l >> r;
  if (op == 3) {
   cout << sqrtdc.sum(l, r) << '\n';
  } else {
   long long b;
   cin >> b;
   if (op == 0) sqrtdc.chmin(l, r, b);
   if (op == 1) sqrtdc.chmax(l, r, b);
   if (op == 2) sqrtdc.add(l, r, b);
  }
 }
 return 0;
}