#define PROBLEM "https://yukicoder.me/problems/no/728"
#include <bits/stdc++.h>
#include "src/DataStructure/KDTree.hpp"
using namespace std;

struct RSQ {
  using T = long long;
  static T ti() { return 0; }
  static T op(T l, T r) { return l + r; }
};
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using KDT = KDTree<2, int, RSQ>;
  int N;
  cin >> N;
  int A[N], L[N], R[N];
  for (int i = 0; i < N; i++) cin >> A[i];
  for (int i = 0; i < N; i++) cin >> L[i] >> R[i];
  vector<typename KDT::PosVal> v(N);
  for (int i = 0; i < N; i++) v[i].first = {A[i], A[i] + R[i]}, v[i].second = 1;
  KDT kdt(v);
  long long ans = 0;
  for (int i = 0; i < N; i++)
    if (L[i] > 0) ans += kdt.fold({A[i] - L[i], A[i] - 1}, {A[i], INT_MAX});
  cout << ans << "\n";
  return 0;
}