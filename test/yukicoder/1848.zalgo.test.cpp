#define PROBLEM "https://yukicoder.me/problems/no/1848"
#include <iostream>
#include <algorithm>
#include "src/String/z_algorithm.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9) + 7>;
 int N;
 cin >> N;
 vector<int> A(N);
 for (int i= 0; i < N; ++i) cin >> A[i];
 string S;
 cin >> S;
 vector<long long> sum(N + 1);
 for (int i= 0; i < N; ++i) sum[i + 1]= sum[i] + A[i];
 vector<pair<char, int>> B(N - 1);
 for (int i= 1; i < N; ++i) B[i - 1]= {S[i], A[i]};
 auto Z= z_algorithm(B);
 Mint ans= 0;
 for (int i= 0; i < N; ++i) {
  if (S[i] != S[0]) continue;
  long long a= A[0], b= A[i], k= min(a, b);
  ans+= k * (k + 1) / 2 + a * (b - k);
  if (A[i] < A[0]) continue;
  if (i == N - 1) break;
  int z= Z[i];
  long long s= sum[z + 1];
  if (i + z + 1 < N && S[1 + z] == S[1 + z + i]) s+= min(A[1 + z], A[1 + z + i]);
  ans+= s - a;
 }
 cout << ans << '\n';
 return 0;
}