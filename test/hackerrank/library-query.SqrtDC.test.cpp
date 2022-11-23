#define PROBLEM "https://www.hackerrank.com/challenges/library-query/problem"

#include <bits/stdc++.h>
#include "src/DataStructure/SortedPerBucket.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int T;
  for (cin >> T; T--;) {
    int N;
    cin >> N;
    vector<int> a(N);
    for (int i = 0; i < N; i++) cin >> a[i];
    SortedPerBucket<int, 50> sqrtdc(a);
    int Q;
    cin >> Q;
    while (Q--) {
      int type;
      cin >> type;
      if (type) {
        int x, k;
        cin >> x >> k, x--;
        sqrtdc.set(x, k);
      } else {
        int x, y, k;
        cin >> x >> y >> k, x--;
        int ok = 0, ng = 1000;
        while (abs(ok - ng) > 1) {
          int t = (ok + ng) / 2;
          (sqrtdc.count(x, y, t) < k ? ok : ng) = t;
        }
        cout << ok << '\n';
      }
    }
  }
  return 0;
}