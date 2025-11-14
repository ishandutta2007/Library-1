// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1170
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 256
#include <iostream>
#include <algorithm>
#include "src/Graph/RangeToRangeGraph.hpp"
#include "src/Graph/StronglyConnectedComponents.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, A, B;
 cin >> N >> A >> B;
 int x[N];
 for (int i= 0; i < N; ++i) cin >> x[i];
 RangeToRangeGraph r2r(N);
 for (int i= 0; i < N; ++i) {
  int l= lower_bound(x, x + N, x[i] + A) - x;
  int r= upper_bound(x, x + N, x[i] + B) - x;
  r2r.add_to_range(i, l, r), r2r.add_from_range(l, r, i);
 }
 // 無向グラフだと思うと全部くっついちゃうので 例えばUFとかは使えない
 StronglyConnectedComponents scc(r2r.graph);
 int C= scc.size();
 int cnt[C];
 fill_n(cnt, C, 0);
 for (int i= 0; i < N; ++i) ++cnt[scc(i)];
 for (int i= 0; i < N; ++i) cout << cnt[scc(i)] << '\n';
 return 0;
}