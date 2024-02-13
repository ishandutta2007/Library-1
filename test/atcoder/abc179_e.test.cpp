#define PROBLEM "https://atcoder.jp/contests/abc179/tasks/abc179_e"
#include <iostream>
#include "src/Graph/FunctionalGraph.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 long long N, X, M;
 cin >> N >> X >> M;
 vector<int> to(M);
 for (long long i= 0; i < M; ++i) to[i]= i * i % M;
 auto [a, b, c]= FunctionalGraph(to).path(X, N);
 long long ans= 0;
 for (int x: b.first) ans+= x;
 ans*= b.second;
 for (int x: a.first) ans+= x;
 for (int x: c.first) ans+= x;
 cout << ans << '\n';
 return 0;
}