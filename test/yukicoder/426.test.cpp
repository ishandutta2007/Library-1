// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/426
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <array>
#include "src/Math/ModInt.hpp"
#include "src/LinearAlgebra/Matrix.hpp"
#include "src/DataStructure/SegmentTree.hpp"
using namespace std;
using Mint= ModInt<int(1e9 + 7)>;
using Mat= Matrix<Mint>;
struct Mono {
 using T= array<Mat, 3>;
 static T op(const T &a, const T &b) { return {b[0] * a[0], a[2] * b[1] * a[0] + a[1], a[2] * b[2]}; }
 static T ti() { return {Mat::identity(3), Mat(2, 3), Mat::identity(2)}; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N;
 cin >> N;
 SegmentTree<Mono> seg(N + 1);
 for (int i= 0; i <= N; ++i) {
  Mint s= 6 * i;
  Mat S= {{s, s + 1, s + 2}, {s + 3, s + 4, s + 5}};
  seg.unsafe_set(i, {Mat::identity(3), S, Mat::identity(2)});
 }
 seg.rebuild();
 Vector<Mint> a(3), b(2);
 cin >> a[0] >> a[1] >> a[2] >> b[0] >> b[1];
 int q;
 cin >> q;
 while (q--) {
  string s;
  int i;
  cin >> s >> i;
  if (s == "a") {
   Mat A(3, 3);
   cin >> A[0][0] >> A[0][1] >> A[0][2] >> A[1][0] >> A[1][1] >> A[1][2] >> A[2][0] >> A[2][1] >> A[2][2];
   auto [X, Y, Z]= seg[i];
   seg.set(i, {A, Y, Z});
  } else if (s == "b") {
   Mat B(2, 2);
   cin >> B[0][0] >> B[0][1] >> B[1][0] >> B[1][1];
   auto [X, Y, Z]= seg[i];
   seg.set(i, {X, Y, B});
  } else if (s == "ga") {
   auto ans= seg.prod(0, i)[0] * a;
   cout << ans[0] << " " << ans[1] << " " << ans[2] << '\n';
  } else {
   auto [X, Y, Z]= seg.prod(i + 1, N + 1);
   auto ans= Z * b + Y * seg.prod(0, i + 1)[0] * a;
   cout << ans[0] << " " << ans[1] << '\n';
  }
 }
 return 0;
}