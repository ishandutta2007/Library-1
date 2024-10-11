// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/yahoo-procon2019-final/tasks/yahoo_procon2019_final_d
#include <sstream>
#include <string>
#include <cassert>
#include <vector>
#include <array>
#include "src/Math/ModInt.hpp"
#include "src/LinearAlgebra/Matrix.hpp"
#include "src/DataStructure/SegmentTree.hpp"
#include "src/Misc/compress.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
using Mint= ModInt<int(1e9 + 7)>;
using Mat= Matrix<Mint>;
struct M {
 using T= Mat;
 static T op(const T& a, const T& b) { return a * b; }
 static T ti() { return Mat::identity(2); }
};
signed main(stringstream& scin, stringstream& scout) {
 int N, Q;
 scin >> N >> Q;
 vector<array<int, 3>> query(Q);
 vector<int> x;
 for (int q= 0; q < Q; ++q) {
  int t;
  scin >> t;
  if (t == 1) {
   int p;
   scin >> p, --p;
   x.push_back(p), x.push_back(p + 1);
   query[q]= {t, p, 0};
  } else {
   int l, r;
   scin >> l >> r, --l;
   x.push_back(l), x.push_back(r);
   query[q]= {t, l, r};
  }
 }
 auto id= compress(x);
 int n= x.size();
 bool f[n - 1];
 fill_n(f, n - 1, false);
 Mat A= {{1, 1}, {1, 0}};
 SegmentTree<M> seg(n - 1, [&](int i) { return A.pow(x[i + 1] - x[i]); });
 for (int q= 0; q < Q; ++q) {
  auto [t, l, r]= query[q];
  if (t == 1) {
   int p= id(l);
   f[p]= !f[p];
   if (f[p]) seg.set(p, Mat{{0, 0}, {1, 0}});
   else seg.set(p, A);
  } else {
   l= id(l), r= id(r);
   scout << seg.prod(l, r)[0][1] << '\n';
  }
 }
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "7 3\n2 4 7\n1 3\n2 1 7\n", "3\n3\n"));
 assert(test(TEST::main, "10 10\n2 2 7\n1 3\n2 2 3\n2 1 2\n1 5\n2 5 8\n2 1 6\n2 3 4\n1 2\n2 1 10\n", "8\n0\n1\n0\n1\n0\n0\n"));
 return 0;
}