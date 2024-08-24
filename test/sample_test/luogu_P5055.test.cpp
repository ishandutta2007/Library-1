// competitive-verifier: STANDALONE

// https://www.luogu.com.cn/problem/P5055
// 全永続, insert, erase, fold, reverse, 強制オンラインクエリ
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
struct Mono {
 using T= long long;
 static T op(T l, T r) { return l + r; }
 using commute= void;
};
signed main(stringstream& scin, stringstream& scout) {
 using WBT= WeightBalancedTree<Mono, 1, 1, 1 << 21>;
 int n;
 scin >> n;
 vector<WBT> wbt(n + 1);
 long long lst= 0;
 for (int i= 1; i <= n; ++i) {
  int v, opt;
  scin >> v >> opt;
  wbt[i]= wbt[v];
  if (opt == 1) {
   long long p, x;
   scin >> p >> x, p^= lst, x^= lst;
   wbt[i].insert(p, x);
  } else if (opt == 2) {
   long long p;
   scin >> p, p^= lst;
   --p;
   wbt[i].erase(p);
  } else if (opt == 3) {
   long long l, r;
   scin >> l >> r, l^= lst, r^= lst;
   --l;
   wbt[i].reverse(l, r);
  } else {
   long long l, r;
   scin >> l >> r, l^= lst, r^= lst;
   --l;
   scout << (lst= wbt[i].prod(l, r)) << '\n';
  }
  if (WBT::pool_empty()) {
   vector<vector<long long>> dmps(i + 1);
   for (int j= 1; j <= i; ++j) dmps[j]= wbt[j].dump();
   WBT::reset();
   for (int j= 1; j <= i; ++j) wbt[j]= WBT(dmps[j]);
  }
 }
 return 0;
}
}
signed main() {
 assert(test(TEST::main,
             "10\n"
             "0 1 0 1\n"
             "1 1 1 2\n"
             "2 4 1 2\n"
             "3 1 2 0\n"
             "4 4 2 1\n"
             "5 3 5 7\n"
             "6 4 5 6\n"
             "4 1 7 1\n"
             "8 3 4 6\n"
             "9 4 4 1\n",
             "3\n"
             "4\n"
             "5\n"
             "10\n"));
 return 0;
}