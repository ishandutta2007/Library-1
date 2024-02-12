#define PROBLEM "https://atcoder.jp/contests/abc241/tasks/abc241_e"
#include <iostream>
#include "src/Graph/FunctionalGraph.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 long long K;
 cin >> N >> K;
 long long A[N];
 for (int i= 0; i < N; ++i) cin >> A[i];
 vector<int> to(N);
 for (int i= 0; i < N; ++i) to[i]= (i + A[i]) % N;
 auto [a, b, c]= FunctionalGraph(to).path(0, K);
 long long ans= 0;
 for (int x: b.first) ans+= A[x];
 ans*= b.second;
 for (int x: a.first) ans+= A[x];
 for (int x: c.first) ans+= A[x];
 cout << ans << '\n';
 return 0;
}