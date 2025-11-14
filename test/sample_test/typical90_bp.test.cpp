// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/typical90/tasks/typical90_bp
// ポテンシャルUF (affine 合成, 非可換群)
#include <sstream>
#include <string>
#include <cassert>
#include "src/DataStructure/UnionFind_Potentialized.hpp"
#include "src/Math/Algebra.hpp"
using namespace std;
bool test(int (*solve)(stringstream &, stringstream &), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
struct M {
 using T= pair<bool, long long>;
 static constexpr T o= {false, 0};
 static T add(const T &a, const T &b) {
  if (b.first) return {!a.first, b.second - a.second};
  else return {a.first, a.second + b.second};
 }
 static T neg(const T &a) { return {a.first, (a.first ? a.second : -a.second)}; }
};
signed main(stringstream &scin, stringstream &scout) {
 using G= Algebra<M>;
 int N, Q;
 scin >> N >> Q;
 UnionFind_Potentialized<G> uf(N);
 while (Q--) {
  int T, X, Y, V;
  scin >> T >> X >> Y >> V, --X, --Y;
  if (T) {
   if (uf.connected(X, Y)) {
    auto [a, b]= uf.diff(Y, X).x;
    scout << (a ? -V : V) + b << '\n';
   } else scout << "Ambiguous" << '\n';
  } else {
   uf.unite(Y, X, G(make_pair(true, V)));
  }
 }
 return 0;
}
}
signed main() {
 assert(test(TEST::main,
             "4\n"
             "7\n"
             "0 1 2 3\n"
             "1 1 2 1\n"
             "1 3 4 5\n"
             "0 3 4 6\n"
             "1 3 4 5\n"
             "0 2 3 6\n"
             "1 3 1 5\n",
             "2\n"
             "Ambiguous\n"
             "1\n"
             "2\n"));
 assert(test(TEST::main,
             "15\n"
             "25\n"
             "0 11 12 41\n"
             "0 1 2 159\n"
             "0 14 15 121\n"
             "0 4 5 245\n"
             "0 12 13 157\n"
             "0 9 10 176\n"
             "0 6 7 170\n"
             "0 2 3 123\n"
             "0 7 8 167\n"
             "0 3 4 159\n"
             "1 12 11 33\n"
             "0 10 11 116\n"
             "0 8 9 161\n"
             "1 9 12 68\n"
             "1 12 12 33\n"
             "1 7 12 74\n"
             "0 5 6 290\n"
             "1 8 9 93\n"
             "0 13 14 127\n"
             "1 10 12 108\n"
             "1 14 1 3\n"
             "1 13 8 124\n"
             "1 12 11 33\n"
             "1 12 10 33\n"
             "1 5 15 194\n",
             "8\n"
             "33\n"
             "33\n"
             "33\n"
             "68\n"
             "33\n"
             "144\n"
             "93\n"
             "8\n"
             "108\n"
             "118\n"));
 return 0;
}