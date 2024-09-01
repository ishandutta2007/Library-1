// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc199/tasks/abc199_d
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Math/set_power_series.hpp"
#include "src/Graph/UndirectedGraphSetPowerSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, M;
 cin >> N >> M;
 UndirectedGraphSetPowerSeries g(N);
 for (int i= 0, A, B; i < M; i++) cin >> A >> B, g.add_edge(--A, --B);
 auto tmp= g.colorings_using_exactly_k_colors_num<long long>();
 long long ans= 0, fact= 1;
 for (int i= 0; i < min(3, N); ++i) fact*= 3 - i, ans+= fact * tmp[i + 1];
 cout << ans << '\n';
 return 0;
}