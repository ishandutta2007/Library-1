// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2345
// competitive-verifier: ERROR 0.000000001
// competitive-verifier: TLE 0.5
// double型, log
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include "src/Math/set_power_series.hpp"
#include "src/Graph/UndirectedGraphSetPowerSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 cout << fixed << setprecision(12);
 int N, M, P;
 cin >> N >> M >> P;
 UndirectedGraphSetPowerSeries g(N);
 for (int i= 0, u, v; i < M; i++) cin >> u >> v, g.add_edge(--u, --v);
 if (P == 0) return cout << (g.connected_component_num().back() == 1) << '\n', 0;
 auto e= g.edge_num();
 vector<double> h(e.size());
 for (int s= e.size(); s--;) h[s]= pow(100. / P, e[s]);
 auto f= sps::log(h);
 cout << f.back() * pow(double(P) / 100, e.back()) << '\n';
 return 0;
}