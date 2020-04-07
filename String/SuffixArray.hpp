/**
 * @title SuffixArray
 * @category 文字列
 * @brief 構築 O(|S| log |S|)
 * @brief lower_bound(T) 文字列Tを含む接尾辞のindexの下限 O(|T| log |S|)
 * @brief LongestCommonPrefix配列（高さ配列） 構築 O(|S|)
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

struct SuffixArray {
  vector<int> SA;
  const string s;
  SuffixArray(const string &str) : s(str) {
    SA.resize(s.size());
    iota(begin(SA), end(SA), 0);
    sort(begin(SA), end(SA),
         [&](int a, int b) { return s[a] == s[b] ? a > b : s[a] < s[b]; });
    vector<int> classes(s.size()), c(s.begin(), s.end()), cnt(s.size());
    for (int len = 1; len < s.size(); len <<= 1) {
      for (int i = 0; i < s.size(); i++) {
        if (i > 0 && c[SA[i - 1]] == c[SA[i]] && SA[i - 1] + len < s.size()
            && c[SA[i - 1] + len / 2] == c[SA[i] + len / 2]) {
          classes[SA[i]] = classes[SA[i - 1]];
        } else {
          classes[SA[i]] = i;
        }
      }
      iota(begin(cnt), end(cnt), 0);
      copy(begin(SA), end(SA), begin(c));
      for (int i = 0; i < s.size(); i++) {
        int s1 = c[i] - len;
        if (s1 >= 0) SA[cnt[classes[s1]]++] = s1;
      }
      classes.swap(c);
    }
  }
  int operator[](int k) const { return (SA[k]); }

  // O(|T|*log|S|)
  int lower_bound(string &T) {
    int low = -1, high = s.size();
    while (high - low > 1) {
      int m = (low + high) >> 1;
      if (s.compare(SA[m], T.length(), T) < 0)
        low = m;
      else
        high = m;
    }
    return high;
  }
  int upper_bound(string &T) {
    T.back()++;
    int res = lower_bound(T);
    T.back()--;
    return res;
  }
  // O(|T|*log|S|)
  int count(string &T) { return upper_bound(T) - lower_bound(T); }

  // O(|S|)
  vector<int> get_lcp() {
    vector<int> rank(SA.size()), LCP(SA.size());
    for (int i = 0; i < SA.size(); i++) rank[SA[i]] = i;
    for (int i = 0, h = 0; i < SA.size(); i++) {
      if (rank[i] + 1 < SA.size()) {
        for (int j = SA[rank[i] + 1];
             max(i, j) + h < SA.size() && s[i + h] == s[j + h]; ++h)
          ;
        LCP[rank[i] + 1] = h;
        if (h > 0) --h;
      }
    }
    return LCP;
  }
};