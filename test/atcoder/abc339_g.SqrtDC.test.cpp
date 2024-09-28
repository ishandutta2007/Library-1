// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc339/tasks/abc339_g
// competitive-verifier: TLE 3.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/DataStructure/SortedPerBucket.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 vector<long long> A(N);
 for (int i= 0; i < N; ++i) cin >> A[i];
 SortedPerBucket spb(A);
 long long B= 0;
 int Q;
 cin >> Q;
 while (Q--) {
  long long a, b, c;
  cin >> a >> b >> c;
  int L= a ^ B, R= b ^ B, X= c ^ B;
  B= spb.sum(L - 1, R, X + 1);
  cout << B << '\n';
 }
 return 0;
}