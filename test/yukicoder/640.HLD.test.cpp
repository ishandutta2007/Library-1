#define PROBLEM "https://yukicoder.me/problems/640"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Graph/HeavyLightDecomposition.hpp"
#include "src/DataStructure/SegmentTree_Lazy.hpp"
#include "src/Math/ModInt.hpp"
#undef call_from_test

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
  static T f(const T &vl, const T &vr) {
    return T(vl.val + vr.val, vl.coef + vr.coef);
  }
  static T g(const T &val, const E &op) {
    return T(val.val + val.coef * op, val.coef);
  }
  static E h(const E &opl, const E &opr) { return opl + opr; }
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
  auto q = [&](int a, int b) { return seg.query(a, b); };
  int Q;
  cin >> Q;
  while (Q--) {
    int op, X, Y;
    cin >> op >> X >> Y;
    X--, Y--;
    if (op) {
      cout << hld.query_path(X, Y, q, Mono::f, Mono::ti()).val << endl;
    } else {
      Mint Z;
      cin >> Z;
      auto upd = [&](int a, int b) { seg.update(a, b, Z); };
      hld.update_path(X, Y, upd);
    }
  }
  return 0;
}