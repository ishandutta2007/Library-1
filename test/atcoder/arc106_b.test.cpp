// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/arc106/tasks/arc106_b
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Graph/incidence_matrix_equation.hpp"
using namespace std;
// 解無しの判定のverify

signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, M;
 cin >> N >> M;
 vector<long long> a(N);
 for (int i= 0; i < N; i++) cin >> a[i];
 for (int i= 0, b; i < N; i++) cin >> b, a[i]-= b;
 Graph g(N, M);
 for (int i= 0; i < M; ++i) cin >> g[i], --g[i];
 if (M) cout << (incidence_matrix_equation(g, a).empty() ? "No" : "Yes") << '\n';
 else cout << (all_of(begin(a), end(a), [](auto t) { return !t; }) ? "Yes" : "No") << '\n';
 return 0;
}