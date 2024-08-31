// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/2332
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Misc/Pointwise.hpp"
#include "src/String/RollingHash.hpp"
#include "src/Misc/rng.hpp"
#include "src/Optimization/LiChaoTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 using K= Pointwise<Mint, Mint>;
 using RH= RollingHash<K>;
 RH::init({rng(), rng()});
 int N, M;
 cin >> N >> M;
 vector<int> A(N), B(M), C(M);
 for (int i= 0; i < N; ++i) cin >> A[i];
 for (int i= 0; i < M; ++i) cin >> B[i];
 for (int i= 0; i < M; ++i) cin >> C[i];
 RH rha(A), rhb(B);
 auto f= [&](int x, long long a, long long b) { return a * x + b; };
 LiChaoTree lct(f, 0, 1e9 + 10);
 auto tree= lct.make_tree<MINIMIZE>();
 long long ans= 0;
 for (int i= 0; i < M; ++i) {
  if (ans >= 0) tree.insert(C[i], ans - (long long)C[i] * i, i + 1, i + lcp(rha, rhb.sub(i)) + 1);
  auto [a, b]= tree.query(i + 1);
  ans= b < 0 ? -1 : a;
 }
 cout << ans << '\n';
 return 0;
}