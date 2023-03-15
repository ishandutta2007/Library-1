#define PROBLEM "https://yukicoder.me/problems/no/519"
#include <iostream>
#include "src/Optimization/WeightedMatching.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n;
 cin >> n;
 WeightedMatching<long long> graph(n);
 for (int i= 0; i < n; ++i)
  for (int j= 0; j < n; ++j) {
   int F;
   cin >> F;
   graph.add_edge(i, j, F);
  }
 graph.build();
 long long ans= 0;
 for (auto [u, v, w]: graph.weight_matching()) ans+= w;
 cout << ans << '\n';
 return 0;
}