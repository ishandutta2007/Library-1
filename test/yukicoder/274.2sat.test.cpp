// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/274
// competitive-verifier: TLE 1
// competitive-verifier: MLE 256
#include <iostream>
#include "src/Misc/TwoSatisfiability.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 int L[N], R[N];
 for (int i= 0; i < N; ++i) cin >> L[i] >> R[i];
 TwoSatisfiability sat(N);
 for (int i= N; i--;)
  for (int j= i; j--;) {
   if (!(R[i] < L[j] || R[j] < L[i])) sat.add_nand(i, j), sat.add_nand(sat.neg(i), sat.neg(j));
   if (!(R[i] < M - 1 - R[j] || M - 1 - L[j] < L[i])) sat.add_nand(sat.neg(i), j), sat.add_nand(i, sat.neg(j));
  }
 cout << (sat.solve().empty() ? "NO" : "YES") << '\n';
 return 0;
}