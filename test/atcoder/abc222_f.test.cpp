#define PROBLEM "https://atcoder.jp/contests/abc222/tasks/abc222_f"
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Graph/rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Tree<long long> tree(N);
 for (int i= 0; i < N - 1; ++i) {
  int A, B, C;
  cin >> A >> B >> C;
  tree.add_edge(--A, --B, C);
 }
 tree.build();
 vector<long long> D(N);
 for (int i= 0; i < N; ++i) cin >> D[i];
 auto f_ee= [&](long long l, long long r) { return max(l, r); };
 auto f_ve= [&](long long d, int, const auto& e) { return max(d, D[e.to]) + e.cost; };
 auto f_ev= [&](long long d, int) { return d; };
 auto dp= rerooting<long long>(tree, f_ee, f_ve, f_ev, 0ll);
 for (long long x: dp) cout << x << '\n';
 return 0;
}