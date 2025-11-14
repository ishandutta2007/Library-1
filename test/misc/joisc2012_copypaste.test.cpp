// competitive-verifier: PROBLEM https://www2.ioi-jp.org/camp/2012/2012-sp-tasks/2012-sp-day4.pdf#2
// competitive-verifier: TLE 2.5
// competitive-verifier: MLE 256

// 融合永続, split, split3
#include <iostream>
#include "src/DataStructure/WeightBalancedTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using WBT= WeightBalancedTree<char, 0, 1>;
 int M;
 cin >> M;
 string str;
 cin >> str;
 WBT wbt(str.data(), str.data() + str.length());
 int N;
 cin >> N;
 while (N--) {
  int a, b, c;
  cin >> a >> b >> c;
  auto [x, y, z]= wbt.split3(a, b);
  tie(x, z)= wbt.split(c);
  wbt= x + y + z;
  if ((int)wbt.size() > M) wbt= wbt.split(M).first;
  if (WBT::pool_empty()) {
   auto dump= wbt.dump();
   WBT::reset();
   wbt= WBT(dump);
  }
 }
 auto ans= wbt.dump();
 for (auto& a: ans) cout << a;
 cout << '\n';
 return 0;
}