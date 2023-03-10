#define PROBLEM "https://www.hackerrank.com/contests/university-codesprint-4/challenges/drawing-rectangles"
#include <iostream>
#include <vector>
#include "src/Graph/BipartiteMatching.hpp"
#include "src/DataStructure/RangeSet.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 static constexpr int N= 300'001;
 BipartiteMatching bm(N, N);
 int n;
 cin >> n;
 vector<RangeSet<int>> ys(N);
 for (int i= 0; i < n; ++i) {
  int x1, y1, x2, y2;
  cin >> x1 >> y1 >> x2 >> y2;
  for (int x= x1; x <= x2; ++x) ys[x].insert(y1, y2);
 }
 for (int x= 0; x < N; ++x)
  for (auto [y1, y2]: ys[x])
   for (int y= y1; y <= y2; ++y) bm.add_edge(x, y);
 bm.build();
 vector<int> l, r;
 for (int i= 0; i < N; ++i)
  if (bm.used_as_vertex_cover_left(i)) l.push_back(i);
 for (int i= 0; i < N; ++i)
  if (bm.used_as_vertex_cover_right(i)) r.push_back(i);
 int L= l.size(), R= r.size();
 cout << L + R << '\n';
 cout << L << '\n';
 for (int i= 0; i < L; ++i) cout << (i ? " " : "") << l[i];
 cout << '\n';
 cout << R << '\n';
 for (int i= 0; i < R; ++i) cout << (i ? " " : "") << r[i];
 cout << '\n';
 return 0;
}