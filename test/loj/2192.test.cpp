// competitive-verifier: PROBLEM https://loj.ac/p/2192
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <iostream>
#include <iomanip>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
#include "src/Misc/Pointwise.hpp"
#include "src/Misc/rng.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n;
 cin >> n;
 Graph g(n, n - 1);
 vector<double> w(n - 1), p(n);
 for (int i= 0; i < n - 1; ++i) cin >> g[i] >> w[i], --g[i], w[i]/= 100;
 for (int i= 0; i < n; ++i) cin >> p[i], p[i]/= 100;
 auto put_edge= [&](int, int e, double d) { return d * w[e]; };
 auto op= [](double l, double r) { return 1 - (1 - l) * (1 - r); };
 auto put_vertex= [&](int v, double d) { return 1 - (1 - d) * (1 - p[v]); };
 double ans= 0;
 for (double x: Rerooting<double>(g, put_edge, op, 0., put_vertex)) ans+= x;
 cout << fixed << setprecision(6) << ans << '\n';
 return 0;
}