#define PROBLEM "https://yukicoder.me/problems/no/2122"
#include <iostream>
#include <string>
#include "src/Graph/FunctionalGraph.hpp"
#include "src/FFT/bostan_mori.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 constexpr int N= 10000;
 using Mint= ModInt<N>;
 string s;
 cin >> s;
 int a= stoi(s);
 long long M, L;
 cin >> M >> L;
 FunctionalGraph graph(N);
 for (int n= 0; n < N; ++n) {
  auto x= linear_recurrence<Mint>({n, 1}, {0, 1}, M);
  if (M & 1) x-= 1;
  graph.add_edge(n, x.val());
 }
 graph.build();
 string ans= to_string(graph.jump(a, L));
 ans= string(4 - ans.length(), '0') + ans;
 cout << ans << '\n';
 return 0;
}