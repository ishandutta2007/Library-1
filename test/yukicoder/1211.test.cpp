// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1211
// competitive-verifier: TLE 2
#include <iostream>
#include "src/Misc/Period.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, K;
 cin >> N >> K;
 long long A[N];
 for (int i= 0; i < N; ++i) cin >> A[i];
 long long sum[N + N + 1];
 sum[0]= 0;
 for (int i= 0; i < N + N; ++i) sum[i + 1]= sum[i] + A[i % N];
 long long ok= 0, ng= 1ll << 50;
 while (ng - ok > 1) {
  long long x= (ng + ok) / 2;
  vector<int> to(N + N + 1);
  for (int i= 0, j= 0; i < N + N + 1; ++i) {
   while (j < N + N && sum[j] - sum[i] < x) ++j;
   to[i]= j;
  }
  Period g(to);
  bool isok= false;
  for (int i= 0; i < N; ++i) isok|= (g.jump(i, K) <= i + N);
  (isok ? ok : ng)= x;
 }
 cout << ok << '\n';
 return 0;
}