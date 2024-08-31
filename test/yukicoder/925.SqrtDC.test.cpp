// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/925
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include "src/DataStructure/SortedPerBucket.hpp"
using namespace __gnu_pbds;
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 vector<long long> a(N);
 for (int i= 0; i < N; i++) cin >> a[i];
 SortedPerBucket<long long, 1024> sqrtdc(a);
 long long s= 0;
 tree<long long, null_type, less<long long>, rb_tree_tag, tree_order_statistics_node_update> S(a.begin(), a.end());
 while (Q--) {
  int op;
  cin >> op;
  if (op == 1) {
   long long X, Y;
   cin >> X >> Y;
   (X^= s)&= (1 << 16) - 1, (Y^= s)&= (1ll << 40) - 1;
   X--;
   sqrtdc.set(X, Y);
   S.insert(Y);
  } else {
   int L, R;
   cin >> L >> R;
   (L^= s)&= (1 << 16) - 1, (R^= s)&= (1 << 16) - 1;
   if (L > R) swap(L, R);
   L--;
   int m= (R - L) / 2;
   int ok= 0, ng= S.size();
   while (abs(ok - ng) > 1) {
    int i= (ok + ng) / 2;
    (sqrtdc.count(L, R, *S.find_by_order(i)) <= m ? ok : ng)= i;
   }
   long long x= *S.find_by_order(ok);
   long long ans= sqrtdc.sum(L, R) - sqrtdc.sum(L, R, x) * 2;
   ans-= x * (R - L - sqrtdc.count(L, R, x) * 2);
   cout << ans << '\n';
   s^= ans;
  }
 }
 return 0;
}