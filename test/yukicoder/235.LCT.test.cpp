#define PROBLEM "https://yukicoder.me/problems/no/235"
#include <bits/stdc++.h>
#include "src/DataStructure/LinkCutTree_MonoidLazy.hpp"
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
  LinkCutTree_MonoidLazy<Mono> lct(N);
  Mint S[N], C[N];
  for (int i = 0; i < N; i++) cin >> S[i];
  for (int i = 0; i < N; i++) cin >> C[i];
  for (int i = 0; i < N; i++) lct.set_val(i, {S[i], C[i]});
  for (int i = 0; i < N - 1; i++) {
    int A, B;
    cin >> A >> B;
    A--, B--;
    lct.link(A, B);
  }
  lct.evert(0);
  int Q;
  cin >> Q;
  while (Q--) {
    int op, X, Y;
    cin >> op >> X >> Y;
    X--, Y--;
    if (op) {
      cout << lct.fold(X, Y).val << endl;
    } else {
      Mint Z;
      cin >> Z;
      lct.update(X, Y, Z);
    }
  }
  return 0;
}