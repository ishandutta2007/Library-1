// competitive-verifier: PROBLEM https://www.hackerrank.com/contests/happy-query-contest/challenges/range-counting-query
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Misc/Mo.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 int A[N], cnt[N]= {}, sum[N + 2]= {};
 for (int i= 0; i < N; i++) cin >> A[i], A[i]--;
 Mo mo;
 int Q;
 cin >> Q;
 int x[Q], y[Q], ans[Q];
 for (int q= 0; q < Q; q++) {
  int l, r;
  cin >> l >> r >> x[q] >> y[q];
  mo.query(--l, r);
 }
 auto add= [&](int k) { sum[++cnt[A[k]]]++; };
 auto erase= [&](int k) { sum[cnt[A[k]]--]--; };
 auto out= [&](int q) { ans[q]= sum[x[q]] - sum[y[q] + 1]; };
 mo.run(add, erase, out);
 for (int q= 0; q < Q; q++) cout << ans[q] << '\n';
 return 0;
}