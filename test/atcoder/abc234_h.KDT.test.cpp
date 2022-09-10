#define PROBLEM "https://atcoder.jp/contests/abc234/tasks/abc234_h"
#include <bits/stdc++.h>
#include "src/DataStructure/KDTree.hpp"

//  円形クエリ
using namespace std;

struct IDs {
  using T = vector<int>;
  static T ti() { return {}; }
  static T op(T l, T r) {
    if (l.size() < r.size()) l.swap(r);
    for (int x : r) l.push_back(x);
    return l;
  }
};
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  long long N, K;
  cin >> N >> K;
  using KDT = KDTree<2, long long, IDs>;
  std::vector<typename KDT::PosVal> v(N);
  for (int i = 0; i < N; i++) {
    long long x, y;
    cin >> x >> y;
    v[i].first = {x, y}, v[i].second = {i + 1};
  }
  KDT kdt(v);
  vector<pair<int, int>> ans;
  for (auto [p, ii] : v) {
    auto [x, y] = p;
    int i = ii[0];
    auto in = [x, y, K](long long pos[2]) {
      long long dx = pos[0] - x, dy = pos[1] - y;
      return dx * dx + dy * dy <= K * K;
    };
    auto inall = [x, y, K](long long range[2][2]) {
      long long dx = max(abs(range[0][0] - x), abs(range[0][1] - x));
      long long dy = max(abs(range[1][0] - y), abs(range[1][1] - y));
      return dx * dx + dy * dy <= K * K;
    };
    auto outall = [x, y, K](long long range[2][2]) {
      long long dx = clamp(x, range[0][0], range[0][1]) - x;
      long long dy = clamp(y, range[1][0], range[1][1]) - y;
      return dx * dx + dy * dy > K * K;
    };
    auto ids = kdt.fold(in, inall, outall);
    for (int j : ids)
      if (i < j) ans.emplace_back(i, j);
  }
  sort(ans.begin(), ans.end());
  cout << ans.size() << '\n';
  for (auto [i, j] : ans) cout << i << " " << j << '\n';
  return 0;
}