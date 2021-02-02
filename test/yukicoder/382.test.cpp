#define PROBLEM "https://yukicoder.me/problems/no/382"
#include <bits/stdc++.h>
#include <src/Graph/MaxClique.hpp>
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  long long S;
  cin >> S;
  auto next = [&]() { return S = (S * 12345) % 1000003; };
  int N = (next() % 120) + 2;
  MaxClique graph(N);
  int P = next();
  for (int i = 0; i < N; i++)
    for (int j = i + 1; j < N; j++)
      if (next() >= P) graph.add_edge(i, j);
  auto ans = graph.get_max_independent_set();
  if (N == (int)ans.size())
    cout << -1 << '\n';
  else {
    cout << ans.size() + 1 << '\n';
    for (int i = 0; i < (int)ans.size(); i++)
      cout << (i ? " " : "") << ans[i] + 1;
    cout << '\n';
  }
  return 0;
}