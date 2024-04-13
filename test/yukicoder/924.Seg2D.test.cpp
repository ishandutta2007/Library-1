#define PROBLEM "https://yukicoder.me/problems/no/924"
#include <iostream>
#include <vector>
#include <array>

// kdt だと TLE

#include "src/DataStructure/SegmentTree_2D.hpp"
#include "src/DataStructure/WaveletMatrix.hpp"
using namespace std;
struct RSQ {
 using T= long long;
 static T ti() { return 0; }
 static T op(T a, T b) { return a + b; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 vector<int> A(N);
 vector<array<int, 3>> xyv(N);
 for (int i= 0; i < N; ++i) cin >> A[i], xyv[i]= {i, A[i], A[i]};
 WaveletMatrix wm(A);
 SegmentTree_2D<int, RSQ> seg(xyv);
 for (int i= 0; i < Q; ++i) {
  int L, R;
  cin >> L >> R, --L;
  int k= (R - L - 1) / 2;
  long long u= wm.kth_smallest(L, R, k);
  long long ans= u * k - seg.prod(L, R, -1e9, u);
  ans+= seg.prod(L, R, u, 1e9) - u * (R - L - k);
  cout << ans << '\n';
 }
 return 0;
}
