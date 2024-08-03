// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/913
#include <iostream>
#include <vector>
#include "src/Optimization/monotone_minima.hpp"
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
 static constexpr long long INF= 1e18;
 vector<long long> ans(N, INF);
 auto rec= [&](auto rec, int L, int R) -> void {
  if (L == R) return;
  int M= (L + R) / 2;
  {
   auto select= [&](int i, int j, int k) { return f(L + i, M + 1 + j) > f(L + i, M + 1 + k); };
   auto r= monotone_minima(M - L, R - M, select);
   long long mn= INF;
   for (int i= L; i < M; ++i) {
    mn= min(mn, f(i, r[i - L] + M + 1));
    ans[i]= min(ans[i], mn);
   }
  }
  {
   auto select= [&](int i, int j, int k) { return f(L + j, M + 1 + i) > f(L + k, M + 1 + i); };
   auto l= monotone_minima(R - M, M + 1 - L, select);
   long long mn= INF;
   for (int i= R; i-- > M;) {
    mn= min(mn, f(l[i - M] + L, i + 1));
    ans[i]= min(ans[i], mn);
   }
  }
  rec(rec, L, M), rec(rec, M + 1, R);
 };
 rec(rec, 0, N);
 for (int i= 0; i < N; ++i) cout << ans[i] << '\n';
 return 0;
}