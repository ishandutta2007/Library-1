// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc241/tasks/abc241_e
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Misc/Period.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 long long K;
 cin >> N >> K;
 long long A[N];
 for (int i= 0; i < N; ++i) cin >> A[i];
 vector<int> to(N);
 for (int i= 0; i < N; ++i) to[i]= (i + A[i]) % N;
 Period p(to);
 vector<long long> sum(N + 1);
 for (int i= N; i--;) sum[p(i) + 1]= A[i];
 for (int i= 0; i < N; ++i) sum[i + 1]+= sum[i];
 long long ans= 0;
 auto [p1, p2, c, p3]= p.path(0, K - 1);
 for (auto [l, r]: p1) ans+= sum[r] - sum[l];
 for (auto [l, r]: p2) ans+= c * (sum[r] - sum[l]);
 for (auto [l, r]: p3) ans+= sum[r] - sum[l];
 cout << ans << '\n';
 return 0;
}