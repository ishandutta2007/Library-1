#define PROBLEM "https://atcoder.jp/contests/arc106/tasks/arc106_b"
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Graph/LinearSystemIncidence.hpp"
using namespace std;
// 解無しの判定のverify

signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, M;
 cin >> N >> M;
 LinearSystemIncidence<long long> graph(N);
 vector<long long> a(N);
 for (int i= 0; i < N; i++) cin >> a[i];
 for (int i= 0, b; i < N; i++) cin >> b, a[i]-= b;
 for (int i= 0, c, d; i < M; i++) cin >> c >> d, graph.add_edge(--c, --d);
 if (M) cout << (graph.solve(a).empty() ? "No" : "Yes") << '\n';
 else cout << (all_of(begin(a), end(a), [](auto t) { return !t; }) ? "Yes" : "No") << '\n';
 return 0;
}