// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/arc030/tasks/arc030_4
// 融合永続, split3, apply, fold
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
struct RaddQ_RsumQ {
 using T= long long;
 using E= long long;
 static T op(T l, T r) { return l + r; }
 static void mp(T& l, E r, size_t sz) { l+= r * sz; }
 static void cp(E& l, E r) { l+= r; }
};
signed main(stringstream& scin, stringstream& scout) {
 using WBT= WeightBalancedTree<RaddQ_RsumQ, 0, 1>;
 int N, Q;
 scin >> N >> Q;
 long long x[N];
 for (int i= 0; i < N; i++) scin >> x[i];
 WBT wbt(x, x + N);
 while (Q--) {
  int op, a, b;
  scin >> op >> a >> b;
  a--;
  if (op == 1) {
   long long v;
   scin >> v;
   wbt.apply(a, b, v);
  } else if (op == 2) {
   int c, d;
   scin >> c >> d;
   c--;
   WBT A, B, C, D;
   tie(A, D, C)= wbt.split3(c, d);
   tie(A, B, C)= wbt.split3(a, b);
   wbt= A + D + C;
  } else {
   scout << wbt.prod(a, b) << '\n';
  }
  if (WBT::pool_empty()) {
   auto dmp= wbt.dump();
   WBT::reset();
   wbt= WBT(dmp);
  }
 }
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "5 5\n1 2 3 4 5\n3 1 5\n1 1 3 1\n3 1 3\n2 1 3 2 4\n3 1 5\n", "15\n9\n20\n"));
 assert(test(TEST::main,
             "10 30\n"
             "-5 -5 -2 -1 2 -2 0 -4 2 5\n"
             "2 9 10 9 10\n"
             "2 3 5 2 4\n"
             "1 2 10 -1\n"
             "2 1 7 1 7\n"
             "3 1 4\n"
             "2 1 6 2 7\n"
             "2 5 8 6 9\n"
             "3 4 8\n"
             "3 1 10\n"
             "3 9 9\n"
             "1 3 8 -1\n"
             "2 4 9 1 6\n"
             "3 2 7\n"
             "1 9 10 -4\n"
             "1 6 9 -5\n"
             "1 4 6 -7\n"
             "3 2 5\n"
             "2 10 10 7 7\n"
             "1 3 4 -10\n"
             "3 4 9\n"
             "3 8 9\n"
             "2 6 7 8 9\n"
             "3 3 5\n"
             "3 3 9\n"
             "1 2 10 -10\n"
             "2 4 6 4 6\n"
             "2 4 9 5 10\n"
             "1 2 6 7\n"
             "3 7 8\n"
             "1 3 6 3\n",
             "-20\n-8\n-18\n1\n-29\n-36\n-78\n-18\n-50\n-86\n-38\n"));
 return 0;
}