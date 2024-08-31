// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/738
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/Misc/compress.hpp"
#include "src/DataStructure/BinaryIndexedTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, K;
 cin >> N >> K;
 long long A[N];
 for (int i= 0; i < N; i++) cin >> A[i];
 vector<long long> vec(A, A + N);
 auto id= compress(vec);
 BinaryIndexedTree<long long> bit1(vec.size()), bit2(vec.size());
 long long sum= 0;
 auto add= [&](int i, int s) {
  int idx= id(A[i]);
  bit1.add(idx, s), bit2.add(idx, s * A[i]), sum+= s * A[i];
 };
 long long ans= 1ll << 60;
 for (int i= 0; i < N; i++) {
  add(i, 1);
  if (i >= K - 1) {
   int med= bit1.find(K / 2);
   int lcnt= bit1.sum(med), hcnt= K - lcnt;
   long long lsum= bit2.sum(med), hsum= sum - lsum;
   long long low= lcnt * vec[med] - lsum;
   long long high= hsum - hcnt * vec[med];
   ans= min(ans, low + high);
   add(i - K + 1, -1);
  }
 }
 cout << ans << "\n";
 return 0;
}