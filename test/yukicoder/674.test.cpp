// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/674
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <algorithm>
#include "src/DataStructure/RangeSet.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 long long D, Q;
 cin >> D >> Q;
 RangeSet<long long, true> rs;
 long long ans= 0;
 while (Q--) {
  long long A, B;
  cin >> A >> B;
  rs.insert(A, B);
  auto cs= rs.covered_by(A, B);
  ans= max(ans, cs.length());
  cout << ans << '\n';
 }
 return 0;
}