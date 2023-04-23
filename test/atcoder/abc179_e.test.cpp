#define PROBLEM "https://atcoder.jp/contests/abc179/tasks/abc179_e"
#include <iostream>
#include "src/Graph/FunctionalGraph.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 long long N, X, M;
 cin >> N >> X >> M;
 FunctionalGraph graph(M);
 for (long long i= 0; i < M; ++i) graph.add_edge(i, i * i % M);
 graph.build();
 auto [a, b, c]= graph.path(X, N);
 long long ans= 0;
 for (int x: b.first) ans+= x;
 ans*= b.second;
 for (int x: a.first) ans+= x;
 for (int x: c.first) ans+= x;
 cout << ans << '\n';
 return 0;
}