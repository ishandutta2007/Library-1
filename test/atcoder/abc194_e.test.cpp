// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc194/tasks/abc194_e
// competitive-verifier: TLE 1.5
// competitive-verifier: MLE 64
#include <iostream>
#include <algorithm>
#include "src/DataStructure/RangeSet.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 int A[N];
 for (int i= 0; i < N; ++i) cin >> A[i];
 RangeSet<int> rs;
 int cnt[N];
 fill_n(cnt, N, 0);
 int r= 0;
 for (; r < M; ++r) rs.insert(A[r]), ++cnt[A[r]];
 int ans= rs.mex(0);
 for (int l= 0; r < N; ++l, ++r) {
  if (--cnt[A[l]] == 0) rs.erase(A[l]);
  if (cnt[A[r]]++ == 0) rs.insert(A[r]);
  ans= min(ans, rs.mex(0));
 }
 cout << ans << '\n';
 return 0;
}