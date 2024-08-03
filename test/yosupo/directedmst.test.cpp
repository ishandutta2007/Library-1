// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/directedmst
#include <iostream>
#include <vector>
#include <numeric>
#include "src/Graph/Graph.hpp"
#include "src/Graph/minimum_spanning_aborescence.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M, S;
 cin >> N >> M >> S;
 Graph g(N, M);
 vector<long long> c(M);
 for (int i= 0; i < M; ++i) cin >> g[i] >> c[i];
 auto [X, es]= minimum_spanning_aborescence(g, c, S);
 vector<int> p(N);
 p[S]= S;
 for (auto e: es) X+= c[e], p[g[e].second]= g[e].first;
 cout << X << '\n';
 for (int i= 0; i < N; ++i) cout << p[i] << " \n"[i + 1 == N];
 return 0;
}