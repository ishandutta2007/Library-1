// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1097
#include <iostream>
#include <vector>
#include "src/Misc/Period.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 vector<int> A(N), to(N);
 for (int i= 0; i < N; ++i) cin >> A[i], to[i]= (i + A[i]) % N;
 Period p(to);
 vector<long long> sum(N + 1);
 for (int i= N; i--;) sum[p(i) + 1]= A[i];
 for (int i= 0; i < N; ++i) sum[i + 1]+= sum[i];
 int Q;
 cin >> Q;
 while (Q--) {
  long long K;
  cin >> K;
  long long ans= 0;
  auto [p1, p2, c, p3]= p.path(0, K - 1);
  for (auto [l, r]: p1) ans+= sum[r] - sum[l];
  for (auto [l, r]: p2) ans+= c * (sum[r] - sum[l]);
  for (auto [l, r]: p3) ans+= sum[r] - sum[l];
  cout << ans << '\n';
 }
 return 0;
}