// competitive-verifier: STANDALONE

// https://www.luogu.com.cn/problem/P5586
// 融合永続, split3, apply, prod, reverse, 強制オンラインクエリ
#include <sstream>
#include <string>
#include <cassert>
#include "src/Math/ModInt.hpp"
#include "src/DataStructure/WeightBalancedTree.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
constexpr int MOD= 1e9 + 7;
using Mint= ModInt<MOD>;
struct Mono {
 using T= Mint;
 using E= int;
 static T op(T l, T r) { return l + r; }
 static void mp(T& a, E b, T sz) {
  if (b <= 0) a= sz * -b;
  else a+= sz * b;
 }
 static void cp(E& a, E b) {
  if (b <= 0) a= b;
  else if (a <= 0) a-= b;
  else a+= b;
  if (a >= MOD) a-= MOD;
  else if (a <= -MOD) a+= MOD;
 }
 using commute= void;
};
signed main(stringstream& scin, stringstream& scout) {
 using WBT= WeightBalancedTree<Mono, 1, 1, 1 << 19>;
 int n, m;
 scin >> n >> m;
 WBT wbt;
 {
  vector<Mint> v(n);
  for (int i= 0; i < n; ++i) scin >> v[i];
  wbt= WBT(v);
 }
 int lst= 0;
 while (m--) {
  int t, l, r;
  scin >> t >> l >> r;
  l^= lst, r^= lst;
  --l;
  if (t == 1) {
   lst= wbt.prod(l, r).val();
   scout << lst << '\n';
  } else if (t == 2) {
   int val;
   scin >> val;
   val^= lst;
   wbt.apply(l, r, -val);
  } else if (t == 3) {
   int val;
   scin >> val;
   val^= lst;
   if (val) wbt.apply(l, r, val);
  } else if (t == 4) {
   int l2, r2;
   scin >> l2 >> r2;
   l2^= lst, r2^= lst;
   --l2;
   auto [a, b, c]= wbt.split3(l, r);
   auto [d, e, f]= wbt.split3(l2, r2);
   wbt= d + b + f;
  } else if (t == 5) {
   int l2, r2;
   scin >> l2 >> r2;
   l2^= lst, r2^= lst;
   --l2;
   if (l2 > l) swap(l, l2), swap(r, r2);
   auto [a, b, c]= wbt.split3(l, r);
   auto [d, e, f]= a.split3(l2, r2);
   wbt= d + b + f + e + c;
  } else {
   wbt.reverse(l, r);
  }
  if (WBT::pool_empty()) {
   auto dmp= wbt.dump();
   WBT::reset();
   wbt= WBT(dmp);
  }
 }
 auto dmp= wbt.dump();
 for (int i= 0; i < n; ++i) scout << dmp[i] << " \n"[i == n - 1];
 return 0;
}
}
signed main() {
 assert(test(TEST::main,
             "10 10\n"
             "7 1 3 2 2 4 0 1 2 2\n"
             "4 10 10 3 3\n"
             "3 4 10 5\n"
             "6 6 7\n"
             "6 9 10\n"
             "1 10 10\n"
             "5 14 13 1 0\n"
             "2 15 13 7\n"
             "5 3 3 2 2\n"
             "5 5 3 15 13\n"
             "3 4 14 7\n",
             "7\n"
             "7 0 0 0 7 7 7 1 2 7\n"));
 return 0;
}