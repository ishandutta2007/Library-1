// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc339/tasks/abc339_g
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
// N=2*10^5, Q=2*10^5, KDTはTLE
#include <iostream>
#include <vector>
#include "src/DataStructure/SegmentTree_2D.hpp"
using namespace std;
struct RSQ {
 using T= long long;
 static T ti() { return 0; }
 static T op(T a, T b) { return a + b; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 vector<tuple<int, int, long long>> ps(N);
 for (int i= 0; i < N; ++i) {
  int A;
  cin >> A;
  ps[i]= {i, A, A};
 }
 SegmentTree_2D<int, RSQ> seg(ps);
 long long B= 0;
 int Q;
 cin >> Q;
 while (Q--) {
  long long a, b, c;
  cin >> a >> b >> c;
  int L= a ^ B, R= b ^ B, X= c ^ B;
  B= seg.prod(L - 1, R, 0, X + 1);
  cout << B << '\n';
 }
 return 0;
}