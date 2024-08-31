// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1371
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/DataStructure/WaveletMatrix.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 vector<int> A(N);
 for (int i= 0; i < N; ++i) cin >> A[i], --A[i];
 if (A[0] < A[1])
  for (auto &x: A) x= N - 1 - x;
 vector<int> B(N);
 for (int i= 0; i < N; ++i) {
  if (i & 1) {
   B[i]= A[i - 1];
   if (i + 1 < N) B[i]= min(B[i], A[i + 1]);
  } else {
   B[i]= 0;
   if (i) B[i]= max(B[i], A[i - 1]);
   if (i + 1 < N) B[i]= max(B[i], A[i + 1]);
  }
 }
 vector<int> ve(N, -1), vo(N, -1);
 for (int i= 0; i < N; ++i) {
  if (i & 1) vo[A[i]]= B[i];
  else ve[A[i]]= B[i];
 }
 WaveletMatrix<int> wme(ve), wmo(vo);
 long long ans= 0;
 for (int i= 0; i < N; ++i) {
  if (i & 1) {
   ans+= wme.count(0, B[i], 0, A[i]);
   ans+= wmo.count(0, B[i], A[i] + 1, N);
  } else {
   ans+= wme.count(B[i] + 1, N, 0, A[i]);
   ans+= wmo.count(B[i] + 1, N, A[i] + 1, N);
  }
 }
 ans= (ans - N) / 2;
 cout << ans << '\n';
 return 0;
}