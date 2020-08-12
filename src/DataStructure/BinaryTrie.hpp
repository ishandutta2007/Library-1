/**
 * @title 二分Trie
 * @category データ構造
 * @brief O(logN)
 * @brief multiset的な仕様(内部は動的セグ木)
 */
// verify用:
// https://codeforces.com/contest/947/problem/C
// https://codeforces.com/contest/966/problem/C

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/DataStructure/SegmentTree_Dynamic.hpp"
#undef call_from_test
#endif

struct RsumQ {
  using T = int;
  static T ti() { return 0; }
  static T f(const T &l, const T &r) { return l + r; }
};
struct BinaryTrie : public SegmentTree_Dynamic<RsumQ> {
  using ll = long long;
  using U = unsigned long long;
  BinaryTrie(ll max_val = INT_MAX) : SegmentTree_Dynamic<RsumQ>(max_val) {}
  void insert(U val) { set_val(val, (*this)[val] + 1); }
  void erase(U val) { set_val(val, (*this)[val] - 1); }
  ll kth_element(int k, U bias = 0) {
    auto check = [&](int x) { return x > k; };
    return find_first(check, bias);
  }
  ll min_element(U bias = 0) { return kth_element(0, bias); }
  ll max_element(U bias = 0) { return kth_element(0, ~bias); }
  int lower_bound(U val) { return query(0, val); }
  int upper_bound(U val) { return query(0, val + 1); }
  int count(U val) { return query(val, val + 1); }
  int size() { return root ? root->dat : 0; }
};