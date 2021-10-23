#define PROBLEM "https://atcoder.jp/contests/abc155/tasks/abc155_f"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Graph/LinearSystemIncidence.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<2>;
  int N, M;
  cin >> N >> M;
  pair<int, bool> AB[N + 2];
  AB[0] = {0, 0}, AB[N + 1] = {1e9 + 10, 0};
  for (int i = 1; i <= N; i++) cin >> AB[i].first >> AB[i].second;
  sort(AB, AB + N + 2);
  vector<Mint> b(N + 1);
  for (int i = 0; i <= N; i++) b[i] = AB[i + 1].second ^ AB[i].second;
  LinearSystemIncidence<Mint> lsi(N + 1);
  for (int i = 0; i < M; i++) {
    int L, R;
    cin >> L >> R;
    L = lower_bound(AB, AB + N + 2, make_pair(L, false)) - AB;
    R = upper_bound(AB, AB + N + 2, make_pair(R, true)) - AB;
    lsi.add_edge(L - 1, R - 1);
  }
  auto [isok, sol] = lsi.solve(b);
  if (isok) {
    vector<int> ans;
    for (int i = 0; i < M; i++)
      if (sol[i] != Mint(0)) ans.emplace_back(i + 1);
    cout << ans.size() << '\n';
    for (int i = 0; i < (int)ans.size(); i++) cout << (i ? " " : "") << ans[i];
    cout << '\n';
  } else
    cout << -1 << '\n';
  return 0;
}