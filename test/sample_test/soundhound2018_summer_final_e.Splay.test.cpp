// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/soundhound2018-summer-final-open/tasks/soundhound2018_summer_final_e
// split3, fold
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
using Mint= ModInt<int(1e9 + 7)>;
struct Mono {
 struct T {
  Mint val;
  int size;
 };
 static T op(const T& l, const T& r) { return {l.val + r.val * Mint(1000000).pow(l.size), l.size + r.size}; }
};
signed main(stringstream& scin, stringstream& scout) {
 using Splay= SplayTree<Mono>;
 int N, M;
 scin >> N >> M;
 vector<Splay> S(M);
 for (int i= 0; i < M; ++i) {
  string str;
  scin >> str;
  vector<Mono::T> v(N);
  for (int i= N; i--;) v[i]= {str[i] - 'a' + 1, 1};
  S[i]= Splay(v);
 }
 int Q;
 scin >> Q;
 while (Q--) {
  int type, x, y, l, r;
  scin >> type >> x >> y >> l >> r;
  --x, --y, --l;
  if (type == 1) {
   auto [xl, xc, xr]= S[x].split3(l, r);
   auto [yl, yc, yr]= S[y].split3(l, r);
   S[x]= xl + yc + xr;
   S[y]= yl + xc + yr;
  } else {
   scout << S[x].prod(l, r).val << '\n';
  }
 }
 return 0;
}
}
signed main() {
 assert(test(TEST::main,
             "5 2\n"
             "abcde\n"
             "fghij\n"
             "8\n"
             "2 1 0 2 3\n"
             "2 2 0 1 5\n"
             "1 1 2 2 2\n"
             "2 1 0 2 3\n"
             "2 2 0 1 5\n"
             "1 1 2 1 3\n"
             "2 1 0 2 3\n"
             "2 2 0 1 5\n",
             "3000002\n"
             "496944447\n"
             "3000007\n"
             "491944447\n"
             "8000002\n"
             "496979442\n"));
 assert(test(TEST::main,
             "7 3\n"
             "pzocuwt\n"
             "ghqsktw\n"
             "ogvyhak\n"
             "13\n"
             "2 1 0 1 2\n"
             "1 1 2 5 6\n"
             "1 1 3 3 6\n"
             "1 2 3 4 5\n"
             "1 2 3 5 6\n"
             "1 1 2 1 6\n"
             "1 1 2 5 6\n"
             "2 2 0 5 5\n"
             "2 1 0 2 3\n"
             "1 2 3 1 4\n"
             "1 1 2 2 7\n"
             "2 3 0 1 6\n"
             "2 3 0 1 4\n",
             "26000016\n"
             "21\n"
             "17000008\n"
             "556958241\n"
             "25847241\n"));
 return 0;
}