// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc292/tasks/abc292_Ex
// competitive-verifier: ERROR 0.000000001
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/abc292/tasks/abc292_h
// max_right
#include <iostream>
#include <iomanip>
#include <vector>
#include "src/DataStructure/SegmentTree.hpp"
using namespace std;
struct M {
 using T= long long;
 using E= long long;
 static T ti() { return -(1ll << 60); }
 static T op(T l, T r) { return max(l, r); }
 static bool mp(T &v, E y, int) { return v+= y, true; }
 static void cp(E &x, E y) { x+= y; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 cout << fixed << setprecision(20);
 int N, B, Q;
 cin >> N >> B >> Q;
 vector<int> a(N);
 for (int i= 0; i < N; ++i) cin >> a[i];
 vector<long long> sum(N);
 sum[0]= a[0] - B;
 for (int i= 1; i < N; ++i) sum[i]= sum[i - 1] + a[i] - B;
 SegmentTree<M> seg(sum);
 while (Q--) {
  int c, x;
  cin >> c >> x, --c;
  int d= x - a[c];
  a[c]= x;
  seg.apply(c, N, d);
  int s= seg.max_right(0, [&](long long x) { return x < 0; });
  if (s == N) s= N - 1;
  cout << (double)seg[s] / (s + 1) + B << '\n';
 }
 return 0;
}