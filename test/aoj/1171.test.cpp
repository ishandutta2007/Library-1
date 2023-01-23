#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1171"
#define ERROR "0.00001"
#include <iostream>
#include <vector>
#include "src/Geometry/!geometry_temp.hpp"
using namespace std;
using namespace geometry;

const Real INF= 1e10;
int N;
vector<Segment> ss;
Point T, L;
Real score(vector<int> seq) {
 vector<Segment> ts= ss;
 Point target= T;
 for (int i: seq) {
  Line l= {ts[i].p1, ts[i].p2};
  for (Segment &t: ts) t= l.reflect(t);
  target= l.reflect(target);
 }
 Segment beam= {L, target};
 ts= ss;
 vector<Point> ps= {L};
 for (int i: seq) {
  auto cp= cross_points(beam, ts[i]);
  if (!cp.size()) return INF;
  if (ps.size() >= 2) {
   auto c= ccw(ps[ps.size() - 2], ps.back(), cp[0]);
   if (c == ONLINE_BACK || c == ON_SEGMENT) return INF;
  }
  ps.push_back(cp[0]);
  Line l= {ts[i].p1, ts[i].p2};
  for (Segment &t: ts) t= l.reflect(t);
 }
 ps.push_back(target);
 ts= ss;
 for (int k= 0; k + 1 < ps.size(); k++) {
  Segment s= {ps[k], ps[k + 1]};
  for (Segment t: ts)
   if (intersect(s, t) == CROSSING) return INF;
  if (k < seq.size()) {
   Line l= {ts[seq[k]].p1, ts[seq[k]].p2};
   for (Segment &t: ts) t= l.reflect(t);
  }
 }
 return dist(L, target);
}
Real rec(vector<int> seq) {
 Real res= score(seq);
 if (seq.size() < 5) {
  for (int i= 0; i < N; i++)
   if (seq.empty() || i != seq.back()) {
    seq.push_back(i);
    res= min(res, rec(seq));
    seq.pop_back();
   }
 }
 return res;
}
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 cout << fixed << setprecision(12);
 for (; cin >> N && N;) {
  ss.resize(N);
  for (int i= 0; i < N; i++) cin >> ss[i].p1 >> ss[i].p2;
  cin >> T >> L;
  cout << rec({}) << endl;
 }
 return 0;
}