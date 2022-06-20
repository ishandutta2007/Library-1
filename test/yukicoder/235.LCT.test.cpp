#define PROBLEM "https://yukicoder.me/problems/no/235"
#include <bits/stdc++.h>
#include "src/DataStructure/LinkCutTree.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;

using Mint = ModInt<int(1e9 + 7)>;

struct Mono {
  struct T {
    Mint val, coef;
  };
  using E = Mint;
  static T op(const T &vl, const T &vr) {
    return {vl.val + vr.val, vl.coef + vr.coef};
  }
  static void mapping(T &val, const E &op) { val.val += val.coef * op; }
  static void composition(E &pre, const E &suf) { pre += suf; }
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N;
  cin >> N;
  LinkCutTree<Mono> lct(N);
  Mint S[N], C[N];
  for (int i = 0; i < N; i++) cin >> S[i];
  for (int i = 0; i < N; i++) cin >> C[i];
  for (int i = 0; i < N; i++) lct.set(i, {S[i], C[i]});
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
      lct.apply(X, Y, Z);
    }
  }
  return 0;
}