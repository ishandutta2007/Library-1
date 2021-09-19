#define PROBLEM "https://yukicoder.me/problems/no/235"
#include <bits/stdc++.h>
#include "src/Graph/HeavyLightDecomposition.hpp"
#include "src/DataStructure/SegmentTree_Lazy.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;

using Mint = ModInt<int(1e9 + 7)>;

struct Mono {
  struct T {
    Mint val;
    Mint coef;
    T(Mint v = 0, Mint s = 1) : val(v), coef(s) {}
  };
  using E = Mint;
  static T ti() { return T(0, 0); }
  static E ei() { return E(0); }
  static T op(const T &vl, const T &vr) {
    return T(vl.val + vr.val, vl.coef + vr.coef);
  }
  static T mapping(const T &val, const E &op) {
    return T(val.val + val.coef * op, val.coef);
  }
  static E composition(const E &opl, const E &opr) { return opl + opr; }
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N;
  cin >> N;
  HeavyLightDecomposition hld(N);
  Mint S[N], C[N];
  for (int i = 0; i < N; i++) cin >> S[i];
  for (int i = 0; i < N; i++) cin >> C[i];
  for (int i = 0; i < N - 1; i++) {
    int A, B;
    cin >> A >> B;
    A--, B--;
    hld.add_edge(A, B);
  }
  hld.build(0);
  SegmentTree_Lazy<Mono> seg(N);
  for (int i = 0; i < N; i++) seg.set_val(hld.in[i], {S[i], C[i]});
  auto q = [&](int a, int b) { return seg.fold(a, b); };
  int Q;
  cin >> Q;
  while (Q--) {
    int op, X, Y;
    cin >> op >> X >> Y;
    X--, Y--;
    if (op) {
      cout << hld.fold_path(X, Y, q, Mono::op, Mono::ti()).val << endl;
    } else {
      Mint Z;
      cin >> Z;
      auto upd = [&](int a, int b) { seg.apply(a, b, Z); };
      hld.apply_path(X, Y, upd);
    }
  }
  return 0;
}