// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1649
// competitive-verifier: TLE 3
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <array>
#include <tuple>

// rectangle sum だと思うと 制約厳しめ
// N=2*10^5, Q=2*10^5 で 2*4*Q回クエリあるみたいなもん
// TL 3 sec (2500ms ぐらいで通る)

#include "src/DataStructure/SegmentTree_2D.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
using Mint= ModInt<998244353>;
struct RSQ {
 using T= array<Mint, 4>;
 static T ti() { return {0, 0, 0, 0}; }
 static T op(const T &l, const T &r) { return {l[0] + r[0], l[1] + r[1], l[2] + r[2], l[3] + r[3]}; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N;
 cin >> N;
 vector<tuple<int, int, array<Mint, 4>>> v(N);
 for (auto &[x, y, c]: v) cin >> x >> y, c= {1, x, y, Mint(x) * y};
 SegmentTree_2D<int, RSQ> seg(v);
 Mint xs= 0, x2s= 0, ys= 0, y2s= 0;
 for (auto &[x, y, c]: v) xs+= x, x2s+= Mint(x) * x, ys+= y, y2s+= Mint(y) * y;
 Mint ans= (x2s + y2s) * N - xs * xs - ys * ys;
 for (auto &[x, y, c]: v) {
  auto [cnt, xs, ys, xys]= seg.prod(0, x, 0, y);
  ans+= (xys - xs * y - ys * x + cnt * x * y) * 2;
 }
 for (auto &[x, y, c]: v) {
  auto [cnt, xs, ys, xys]= seg.prod(x, 0x7fffffff, 0, y);
  ans-= (xys - xs * y - ys * x + cnt * x * y) * 2;
 }
 cout << ans << '\n';
 return 0;
}
