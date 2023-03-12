#define PROBLEM "https://yukicoder.me/problems/no/650"
#include <iostream>

#include "src/Math/ModInt.hpp"
#include "src/LinearAlgebra/Matrix.hpp"
#include "src/DataStructure/LinkCutTree.hpp"
using namespace std;

using Mint= ModInt<int(1e9) + 7>;
using Mat= Matrix<Mint>;
const Mat I= Mat::identity_matrix(2);
struct Monoid {
 using T= Mat;
 static T ti() { return I; }
 static T op(const T &l, const T &r) { return l * r; }
};
int main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int n;
 cin >> n;
 LinkCutTree<Monoid> lct(n + n - 1, I);
 for (int i= 0; i < n - 1; i++) {
  int a, b;
  cin >> a >> b;
  lct.link(a, i + n);
  lct.link(i + n, b);
 }
 int q;
 cin >> q;
 while (q--) {
  char t;
  cin >> t;
  if (t == 'x') {
   int i;
   cin >> i;
   Mat X(2, 2);
   cin >> X[0][0] >> X[0][1] >> X[1][0] >> X[1][1];
   lct.set(i + n, X);
  } else {
   int i, j;
   cin >> i >> j;
   auto ans= lct.fold(i, j);
   cout << ans[0][0] << " " << ans[0][1] << " " << ans[1][0] << " " << ans[1][1] << '\n';
  }
 }
 return 0;
}
