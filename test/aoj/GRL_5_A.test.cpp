#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/5/GRL_5_A"
#include <iostream>
#include <algorithm>
#include <vector>
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n;
 cin >> n;
 Graph g(n - 1);
 vector<int> w(n - 1);
 for (int i= 0; i < n - 1; ++i) cin >> g[i] >> w[i];
 g.build(n, 0);
 auto put_edge= [&](int, int e, int d) { return d + w[e]; };
 auto op= [&](int l, int r) { return max(l, r); };
 auto put_vertex= [&](int, int d) { return d; };
 Rerooting<int> dp(g, put_edge, op, 0, put_vertex);
 cout << *max_element(dp.begin(), dp.end()) << '\n';
 return 0;
}