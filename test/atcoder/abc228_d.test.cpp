#define PROBLEM "https://atcoder.jp/contests/abc228/tasks/abc228_d"
#include <iostream>
#include <map>
#include "src/DataStructure/RangeSet.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int Q;
 cin >> Q;
 RangeSet<int> rs;
 map<int, long long> A;
 constexpr int M= (1 << 20) - 1;
 while (Q--) {
  long long t, x;
  cin >> t >> x;
  int h= x & M;
  auto cs= rs.covered_by(h);
  if (t == 2) cout << (cs ? A[h] : -1) << '\n';
  else {
   if (cs) {
    h= (cs.r + 1) & M, cs= rs.covered_by(h);
    if (cs) h= (cs.r + 1) & M;
   }
   rs.insert(h), A[h]= x;
  }
 }
 return 0;
}