// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/913
// competitive-verifier: TLE 1
// competitive-verifier: MLE 256
#include <iostream>
#include <vector>
#include "src/Optimization/LiChaoTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 long long A[N], sum[N + 1];
 sum[0]= 0;
 for (int i= 0; i < N; ++i) cin >> A[i], sum[i + 1]= sum[i] + A[i];
 auto f= [&](int i, int j) { return (long long)(j - i) * (j - i) + sum[j] - sum[i]; };
 auto g= [&](int i, int j) { return f(j, i); };
 LiChaoTree lct1(f, 0, N + 1);
 LiChaoTree lct2(g, 0, N + 1);
 static constexpr long long INF= 1e18;
 vector<long long> ans(N, INF);
 auto rec= [&](auto rec, int L, int R) -> void {
  if (L == R) return;
  int M= (L + R) / 2;
  {
   auto tree= lct1.make_tree<MINIMIZE>();
   for (int i= M + 1; i <= R; ++i) tree.insert(i);
   long long mn= INF;
   for (int i= L; i < M; ++i) {
    mn= min(mn, tree.query(i).first);
    ans[i]= min(ans[i], mn);
   }
  }
  {
   auto tree= lct2.make_tree<MINIMIZE>();
   for (int i= L; i <= M; ++i) tree.insert(i);
   long long mn= INF;
   for (int i= R; i-- > M;) {
    mn= min(mn, tree.query(i + 1).first);
    ans[i]= min(ans[i], mn);
   }
  }
  rec(rec, L, M), rec(rec, M + 1, R);
 };
 rec(rec, 0, N);
 for (int i= 0; i < N; ++i) cout << ans[i] << '\n';
 return 0;
}