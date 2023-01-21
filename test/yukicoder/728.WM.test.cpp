#define PROBLEM "https://yukicoder.me/problems/no/728"
#include <iostream>
#include <vector>
#include "src/DataStructure/WaveletMatrix.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 int A[N], L[N], R[N];
 for (int i= 0; i < N; i++) cin >> A[i];
 for (int i= 0; i < N; i++) cin >> L[i] >> R[i];
 vector<int> r(N);
 for (int i= 0; i < N; i++) r[i]= A[i] + R[i];
 WaveletMatrix wm(r);
 long long ans= 0;
 for (int i= 0; i < N; i++) {
  int left= lower_bound(A, A + N, A[i] - L[i]) - A;
  ans+= wm.count(left, i, A[i], 0x7fffffff);
 }
 cout << ans << "\n";
 return 0;
}