#define PROBLEM "https://yukicoder.me/problems/no/649"
#include <bits/stdc++.h>
#include "src/DataStructure/SegmentTree_Dynamic.hpp"
#include "src/DataStructure/BinaryTrie.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int Q, K;
  cin >> Q >> K;
  K--;
  BinaryTrie S((long long)1e18 + 10);
  while (Q--) {
    long long v;
    cin >> v;
    if (v == 1) {
      cin >> v;
      S.insert(v);
    } else {
      v = S.kth_element(K);
      cout << v << endl;
      if (v >= 0) S.erase(v);
    }
  }
  return 0;
}