#define PROBLEM "https://atcoder.jp/contests/abc256/tasks/abc256_f"

// 双対 の verify

#include <iostream>
#include <array>
#include "src/Math/ModInt.hpp"
#include "src/DataStructure/WeightBalancedTree.hpp"
using namespace std;

using Mint= ModInt<998244353>;
struct Mono {
 struct T {
  Mint val, coef[2];
  T()= default;
  T(Mint id, Mint v): val(v), coef{(id + 1) * (id + 2) / 2, (id * 2 + 3) / 2} {}
 };
 using E= array<Mint, 3>;
 static void mapping(T &x, const E &mapp, int) { x.val+= mapp[0] * x.coef[0] - mapp[1] * x.coef[1] + mapp[2]; }
 static void composition(E &pre, const E &suf) { pre[0]+= suf[0], pre[1]+= suf[1], pre[2]+= suf[2]; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, Q;
 cin >> N >> Q;
 Mint A[N], D[N];
 for (int i= 0; i < N; i++) cin >> A[i], D[i]= A[i];
 for (int j= 0; j < 3; j++)
  for (int i= 1; i < N; i++) D[i]+= D[i - 1];
 WeightBalancedTree<Mono> wbt(N);
 for (int i= 0; i < N; i++) wbt.set(i, {i, D[i]});
 while (Q--) {
  int op, x;
  cin >> op >> x, x--;
  if (op == 1) {
   Mint v;
   cin >> v, v-= A[x], A[x]+= v;
   wbt.apply(x, N, {v, v * x, v * x * x / 2});
   if (wbt.percentage_used() > 90) wbt.rebuild();
  } else {
   cout << wbt[x].val << '\n';
  }
 }
 return 0;
}