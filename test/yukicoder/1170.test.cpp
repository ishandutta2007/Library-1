#define PROBLEM "https://yukicoder.me/problems/no/1170"
#include <iostream>
#include <algorithm>
#include "src/Graph/Range2RangeGraph.hpp"
#include "src/Graph/StronglyConnectedComponents.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, A, B;
 cin >> N >> A >> B;
 int x[N];
 for (int i= 0; i < N; ++i) cin >> x[i];
 Range2RangeGraph r2r(N);
 for (int i= 0; i < N; ++i) {
  int l= lower_bound(x, x + N, x[i] + A) - x;
  int r= upper_bound(x, x + N, x[i] + B) - x;
  r2r.add_to_range(i, l, r), r2r.add_from_range(l, r, i);
 }
 int n= r2r.node_size();
 // 単純な連結だと思うと全部くっついちゃうので 例えばUFとかは使えない
 StronglyConnectedComponents scc(n);
 for (auto [u, v, _]: r2r.get_edges()) scc.add_edge(u, v);
 auto blks= scc.get_block();
 auto index= scc.get_index(blks);
 int C= blks.size();
 int cnt[C];
 fill_n(cnt, C, 0);
 for (int i= 0; i < N; ++i) ++cnt[index[i]];
 for (int i= 0; i < N; ++i) cout << cnt[index[i]] << '\n';
 return 0;
}