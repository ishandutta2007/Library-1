// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Final/0425
// competitive-verifier: TLE 0.5
#include <iostream>
#include <numeric>
#include <algorithm>
#include "src/Misc/Mo.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, K, Q;
 cin >> N >> K >> Q;
 int a[K], b[K];
 for (int k= 0; k < K; k++) cin >> a[k] >> b[k], a[k]--, b[k]--;
 Mo mo;
 int op[Q], x[Q];
 for (int q= 0; q < Q; q++) {
  int s, t;
  cin >> op[q] >> s >> t >> x[q], x[q]--;
  mo.query(--s, t);
 }
 int ord[N], pos[N];
 iota(ord, ord + N, 0), iota(pos, pos + N, 0);
 auto mover= [&](int k) { swap(ord[a[k]], ord[b[k]]), swap(pos[ord[a[k]]], pos[ord[b[k]]]); };
 auto movel= [&](int k) { swap(pos[a[k]], pos[b[k]]), swap(ord[pos[a[k]]], ord[pos[b[k]]]); };
 int ans[Q];
 auto out= [&](int q) { ans[q]= (op[q] == 1 ? ord[x[q]] : pos[x[q]]) + 1; };
 mo.run(movel, mover, movel, mover, out);
 for (int q= 0; q < Q; q++) cout << ans[q] << "\n";
 return 0;
}