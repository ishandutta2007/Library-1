// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/code-festival-2014-exhibition/tasks/code_festival_exhibition_b
// split3, prod
#include <sstream>
#include <string>
#include <cassert>
#include "src/Math/ModInt.hpp"
#include "src/DataStructure/SplayTree.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
struct M {
 using T= array<int, 2>;
 static T op(const T& l, const T& r) {
  if (l[1] > r[0]) return {l[0], r[1] + l[1] - r[0]};
  else return {l[0] + r[0] - l[1], r[1]};
 }
};
signed main(stringstream& scin, stringstream& scout) {
 using SPLAY= SplayTree<M>;
 int Q;
 scin >> Q;
 SPLAY splay;
 {
  string S;
  scin >> S;
  int N= S.length();
  vector<typename M::T> v(N);
  for (int i= N; i--;) v[i]= {S[i] == ')', S[i] == '('};
  splay= SPLAY(v);
 }
 while (Q--) {
  char x;
  int y, z;
  scin >> x >> y >> z;
  --y;
  if (x == '(') {
   splay.insert(y, {0, 1});
  } else if (x == ')') {
   splay.insert(y, {1, 0});
  } else if (x == 'D') {
   splay.erase(y);
  } else {
   auto [l, r]= splay.prod(y, z);
   scout << l + r << '\n';
  }
 }
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "5\n(()\nD 1 0\n( 3 0\nQ 1 2\n) 1 0\nQ 2 4\n", "0\n1\n"));
 assert(test(TEST::main,
             "11\n"
             "(()()(()\n"
             "( 1 0\n"
             "( 1 0\n"
             "D 4 0\n"
             "Q 2 6\n"
             ") 5 0\n"
             "D 8 0\n"
             "Q 1 9\n"
             "( 3 0\n"
             ") 8 0\n"
             "D 10 0\n"
             "Q 5 10\n",
             "1\n1\n4\n"));
 return 0;
}