#define PROBLEM "https://atcoder.jp/contests/abc256/tasks/abc256_f"

#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/DataStructure/SegmentTree_Dual.hpp"
using namespace std;

using Mint = ModInt<998244353>;
struct Mono {
  struct T {
    int id;
    Mint val;
  };
  using E = array<Mint, 3>;
  static void mapping(T& x, const E& mapp) {
    x.val += mapp[0] * (x.id + 1) * (x.id + 2) / 2 -
             mapp[1] * (2 * x.id + 3) / 2 + mapp[2];
  }
  static void composition(E& pre, const E& suf) {
    pre[0] += suf[0], pre[1] += suf[1], pre[2] += suf[2];
  }
};
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Seg = SegmentTree_Dual<Mono>;
  int N, Q;
  cin >> N >> Q;
  Mint A[N], D[N];
  for (int i = 0; i < N; i++) cin >> A[i], D[i] = A[i];
  for (int j = 0; j < 3; j++)
    for (int i = 1; i < N; i++) D[i] += D[i - 1];
  Seg seg;
  for (int i = 0; i < N; i++) seg.set(i, {i, D[i]});
  while (Q--) {
    int op, x;
    cin >> op >> x, x--;
    if (op == 1) {
      Mint v;
      cin >> v, v -= A[x], A[x] += v;
      seg.apply(x, N, {v, v * x, v * x * x / 2});
    } else {
      cout << seg[x].val << '\n';
    }
  }
  return 0;
}