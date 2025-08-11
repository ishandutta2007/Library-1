// competitive-verifier: PROBLEM https://www.hackerrank.com/contests/university-codesprint-4/challenges/drawing-rectangles
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Graph/BipartiteGraph.hpp"
#include "src/Graph/DulmageMendelsohn.hpp"
#include "src/DataStructure/RangeSet.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 static constexpr int N= 300'001;
 vector<RangeSet<int>> ys(N);
 int n;
 cin >> n;
 for (int i= 0; i < n; ++i) {
  int x1, y1, x2, y2;
  cin >> x1 >> y1 >> x2 >> y2;
  for (int x= x1; x <= x2; ++x) ys[x].insert(y1, y2);
 }
 BipartiteGraph bm(N, N);
 for (int x= 0; x < N; ++x)
  for (auto [y1, y2]: ys[x])
   for (int y= y1; y <= y2; ++y) bm.add_edge(x, y + N);
 DulmageMendelsohn dm(bm);
 auto vertex_cover= dm.min_vertex_cover();
 vector<int> l, r;
 for (int i: vertex_cover)
  if (i < N) l.push_back(i);
  else r.push_back(i - N);
 int L= l.size(), R= r.size();
 cout << L + R << '\n';
 cout << L << '\n';
 for (int i= 0; i < L; ++i) cout << l[i] << " \n"[i + 1 == L];
 cout << R << '\n';
 for (int i= 0; i < R; ++i) cout << r[i] << " \n"[i + 1 == R];
 return 0;
}