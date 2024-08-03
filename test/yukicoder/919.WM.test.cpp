// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/919
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
 for (int i= 0; i < N; i++) cin >> A[i];
 WaveletMatrix wm(A);
 long long ans= 0;
 for (int K= 1; K <= N; K++) {
  int M= N / K, med= (K - 1) / 2;
  long long L[M + 1], R[M + 1], Lsum= 0, Rsum= 0;
  L[0]= R[0]= 0;
  for (int i= 1; i <= M; i++) {
   Lsum+= wm.kth_smallest(K * (i - 1), K * i, med);
   Rsum+= wm.kth_smallest(N - K * i, N - K * (i - 1), med);
   L[i]= max(L[i - 1], Lsum);
   R[i]= max(R[i - 1], Rsum);
  }
  for (int i= 0; i <= M; i++) ans= max(ans, (L[i] + R[M - i]) * K);
 }
 cout << ans << "\n";
 return 0;
}