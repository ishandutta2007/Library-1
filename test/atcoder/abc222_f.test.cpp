#define PROBLEM "https://atcoder.jp/contests/abc222/tasks/abc222_f"
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Graph g(N - 1);
 vector<int> C(N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i] >> C[i], --g[i];
 g.build(N, 0);
 vector<long long> D(N);
 for (int i= 0; i < N; ++i) cin >> D[i];
 auto put_edge= [&](int v, int e, long long d) { return max(d, D[g[e] - v]) + C[e]; };
 auto op= [&](long long l, long long r) { return max(l, r); };
 auto put_vertex= [&](int, long long d) { return d; };
 for (long long x: Rerooting<long long>(g, put_edge, op, 0, put_vertex)) cout << x << '\n';
 return 0;
}