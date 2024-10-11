// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/arc027/tasks/arc027_4
// matrix on segtree (ML ギリギリ)
#include <sstream>
#include <string>
#include <cassert>
#include "src/DataStructure/SegmentTree.hpp"
#include "src/Math/ModInt.hpp"
#include "src/LinearAlgebra/Matrix.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
using Mint= ModInt<int(1e9 + 7)>;
using Mat= Matrix<Mint>;
struct Mono {
 using T= Mat;
 static T ti() { return Mat::identity(10); }
 static T op(const T& l, const T& r) { return l * r; }
};
signed main(stringstream& scin, stringstream& scout) {
 int N;
 scin >> N;
 SegmentTree<Mono> seg(N);
 Mat tmp(10, 10);
 for (int j= 0; j + 1 < 10; j++) tmp[j + 1][j]= 1;
 for (int i= 0; i < N; i++) {
  int h;
  scin >> h;
  Mat m= tmp;
  for (int j= 0; j < h; j++) m[0][j]= 1;
  seg.unsafe_set(i, m);
 }
 seg.build();
 int D;
 scin >> D;
 while (D--) {
  int s, t;
  scin >> s >> t;
  s--, t--;
  scout << seg.prod(s, t)[0][0] << '\n';
 }
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "7\n1 2 3 2 2 1 1\n3\n2 6\n5 7\n1 7\n", "6\n2\n9\n"));
 assert(test(TEST::main, "11\n3 1 4 1 5 9 2 6 5 3 5\n4\n3 7\n2 9\n1 10\n1 11\n", "6\n22\n90\n175\n"));
 return 0;
}