#define PROBLEM "https://atcoder.jp/contests/abc256/tasks/abc256_d"
#include <iostream>
#include "src/DataStructure/RangeSet.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 RangeSet<int> rs;
 for (int i= 0; i < N; ++i) {
  int L, R;
  cin >> L >> R;
  rs.insert(L, R - 1);
 }
 for (auto [l, r]: rs) cout << l << " " << r + 1 << '\n';
 return 0;
}
