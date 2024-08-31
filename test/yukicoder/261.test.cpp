// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/261
// competitive-verifier: TLE 1
#include <iostream>
#include <vector>
#include <numeric>
#include "src/Math/DiscreteLogarithm.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 auto f= [](const vector<int> &l, const vector<int> &r) {
  const int n= l.size();
  vector<int> ret(n);
  for (int i= 0; i < n; ++i) ret[i]= r[l[i]];
  return ret;
 };
 DiscreteLogarithm log(f, f, [](const vector<int> &x) { return x[0]; }, 1e9);
 vector<int> p(N);
 iota(p.begin(), p.end(), 0);
 int K;
 cin >> K;
 while (K--) {
  int X, Y;
  cin >> X >> Y;
  swap(p[--X], p[--Y]);
 }
 int Q;
 cin >> Q;
 while (Q--) {
  vector<int> A(N);
  for (int i= 0; i < N; ++i) cin >> A[i], --A[i];
  auto ans= log(p, p, A) + 1;
  cout << (ans ? ans : -1) << '\n';
 }
 return 0;
}