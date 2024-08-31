// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/409
// competitive-verifier: TLE 0.5
#include <iostream>
#include <algorithm>
#include "src/Optimization/LiChaoTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 long long N, A, B, W;
 cin >> N >> A >> B >> W;
 long long D[N];
 for (int i= 0; i < N; ++i) cin >> D[i];
 auto w= [&](int i, int j, long long d) { return d + D[i - 1] + B * (i - j) * (i - j - 1) / 2 - A * (i - j - 1); };
 LiChaoTree lct(w, 1, N + 1);
 auto tree= lct.make_tree<MINIMIZE>();
 long long ans= 1e18;
 for (int i= 0; i <= N; ++i) {
  auto dp= i ? tree.query(i).first : 0;
  tree.insert(i, dp);
  ans= min(ans, dp + B * (N - i) * (N - i + 1) / 2 - A * (N - i));
 }
 cout << ans + W << '\n';
 return 0;
}