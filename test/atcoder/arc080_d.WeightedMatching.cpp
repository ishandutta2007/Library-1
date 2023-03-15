#define PROBLEM "https://atcoder.jp/contests/arc080/tasks/arc080_d"
#include <iostream>
#include <vector>
#include "src/Math/is_prime.hpp"
#include "src/Optimization/WeightedMatching.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 int x[N];
 for (int i= 0; i < N; ++i) cin >> x[i];
 vector<int> vs= {x[0]};
 for (int i= 1; i < N; ++i)
  if (x[i] - x[i - 1] > 1) vs.push_back(x[i - 1] + 1), vs.push_back(x[i]);
 vs.push_back(x[N - 1] + 1);
 int m= vs.size();
 WeightedMatching<long long, true> graph(m);
 for (int i= m; --i;)
  for (int j= i; j--;) {
   int k= vs[i] - vs[j];
   graph.add_edge(i, j, k & 1 ? is_prime(k) ? 1 : 3 : 2);
  }
 graph.build();
 long long ans= 0;
 for (auto [u, v, w]: graph.weight_matching()) ans+= w;
 cout << ans << '\n';
 return 0;
}