#define PROBLEM "https://yukicoder.me/problems/no/235"
#include <iostream>
#include "src/Graph/HeavyLightDecomposition.hpp"
#include "src/DataStructure/SegmentTree_Beats.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;

using Mint= ModInt<int(1e9 + 7)>;
struct Mono {
 struct T {
  Mint val, coef;
 };
 using E= Mint;
 static T ti() { return {0, 0}; }
 static T op(const T &vl, const T &vr) { return {vl.val + vr.val, vl.coef + vr.coef}; }
 static bool mapping(T &v, const E &f) { return v.val+= v.coef * f, true; }
 static void composition(E &pre, const E &suf) { pre+= suf; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 HeavyLightDecomposition hld(N);
 Mint S[N], C[N];
 for (int i= 0; i < N; i++) cin >> S[i];
 for (int i= 0; i < N; i++) cin >> C[i];
 for (int i= 0; i < N - 1; i++) {
  int A, B;
  cin >> A >> B;
  A--, B--;
  hld.add_edge(A, B);
 }
 hld.build(0);
 SegmentTree_Beats<Mono> seg(N);
 for (int i= 0; i < N; i++) seg.unsafe_set(hld[i], {S[i], C[i]});
 seg.rebuild();
 auto q= [&](int a, int b) { return seg.fold(a, b); };
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
   auto upd= [&](int a, int b) { seg.apply(a, b, Z); };
   hld.apply_path(X, Y, upd);
  }
 }
 return 0;
}