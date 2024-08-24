// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/joisc2012/tasks/joisc2012_copypaste
// 融合永続, split, split3
#include <sstream>
#include <string>
#include <cassert>
#include "src/DataStructure/WeightBalancedTree.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
signed main(stringstream& scin, stringstream& scout) {
 using WBT= WeightBalancedTree<char, 0, 1>;
 int M;
 scin >> M;
 string str;
 scin >> str;
 WBT wbt(str.data(), str.data() + str.length());
 int N;
 scin >> N;
 while (N--) {
  int a, b, c;
  scin >> a >> b >> c;
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
 for (auto& a: ans) scout << a;
 scout << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "18\ncopypaste\n4\n3 6 8\n1 5 2\n4 12 1\n17 18 0\n", "acyppypastoopyppyp\n"));
 assert(test(TEST::main, "100\njoi\n3\n0 1 0\n3 4 3\n2 3 3\n", "jjooii\n"));
 return 0;
}