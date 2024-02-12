#define PROBLEM "https://yukicoder.me/problems/no/1211"
#include <iostream>
#include "src/Graph/FunctionalGraph.hpp"
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
 long long l= 0, h= 1ll << 50;
 while (h - l > 1) {
  long long x= (h + l) / 2;
  vector<int> to(N + N + 1);
  for (int i= 0, j= 0; i < N + N + 1; ++i) {
   while (j < N + N && sum[j] - sum[i] < x) ++j;
   to[i]= j;
  }
  FunctionalGraph g(to);
  bool isok= false;
  for (int i= 0; i < N; ++i) isok|= (g.jump(i, K) <= i + N);
  (isok ? l : h)= x;
 }
 cout << l << '\n';
 return 0;
}