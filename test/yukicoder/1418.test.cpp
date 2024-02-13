#define PROBLEM "https://yukicoder.me/problems/no/1418"
#include <iostream>
#include <array>
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Graph g(N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i], --g[i];
 g.build(N, 0);
 using Data= array<long long, 2>;
 auto put_edge= [&](int, int, const Data &d) { return d; };
 auto op= [&](const Data &l, const Data &r) { return Data{l[0] + r[0], l[1] + r[1]}; };
 auto put_vertex= [&](int, Data d) { return ++d[0], d[1]+= d[0], d; };
 long long ans= 0;
 for (auto [_, x]: Rerooting<Data>(g, put_edge, op, Data{0, 0}, put_vertex)) ans+= x;
 cout << ans << '\n';
 return 0;
}