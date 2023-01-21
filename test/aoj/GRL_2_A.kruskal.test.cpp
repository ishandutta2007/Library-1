#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/2/GRL_2_A"
#include <iostream>
#include <algorithm>
#include <numeric>
#include "src/DataStructure/UnionFind.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, M;
 cin >> N >> M;
 UnionFind uf(N);
 int s[N], t[N];
 long long w[N];
 for (int i= 0; i < M; i++) cin >> s[i] >> t[i] >> w[i];
 long long ans= 0;
 int ord[N];
 iota(ord, ord + N, 0), sort(ord, ord + N, [&](int l, int r) { return w[l] < w[r]; });
 for (int i: ord)
  if (uf.unite(s[i], t[i])) ans+= w[i];
 cout << ans << '\n';
 return 0;
}