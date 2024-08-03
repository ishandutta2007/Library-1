// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc179/tasks/abc179_e
#include <iostream>
#include "src/Misc/Period.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 long long N, X, M;
 cin >> N >> X >> M;
 vector<int> to(M);
 for (long long i= 0; i < M; ++i) to[i]= i * i % M;
 Period p(to);
 vector<long long> sum(M + 1);
 for (int i= M; i--;) sum[p(i) + 1]= i;
 for (int i= 0; i < M; ++i) sum[i + 1]+= sum[i];
 long long ans= 0;
 auto [p1, p2, c, p3]= p.path(X, N - 1);
 for (auto [l, r]: p1) ans+= sum[r] - sum[l];
 for (auto [l, r]: p2) ans+= (sum[r] - sum[l]) * c;
 for (auto [l, r]: p3) ans+= sum[r] - sum[l];
 cout << ans << '\n';
 return 0;
}