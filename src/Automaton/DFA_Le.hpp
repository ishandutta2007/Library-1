#pragma once
#include <bits/stdc++.h>
/**
 * @title $N$以下の非負整数を受理するDFA
 * @category オートマトン
 * @brief 0-fill
 */

// BEGIN CUT HERE

template <class S = int>
struct DFA_Le {
  using state_t = int;
  using symbol_t = S;
  DFA_Le(std::vector<symbol_t> str_, std::set<symbol_t> alp_)
      : str(std::move(str_)), alp(std::move(alp_)) {}
  DFA_Le(const std::string &n, int m, int len = 0) {  // n : m-ary notation
    assert(2 <= m && m <= 10);
    for (int l = n.length(), i = l; i--; len--) str.push_back(n[i] - '0');
    while (len-- > 0) str.push_back(0);
    while (m--) alp.insert(m);
  }
  template <class Int>  // m-ary number
  DFA_Le(Int n, int m, int len = 0) {
    for (; n; n /= m, len--) str.push_back(n % m);
    while (len-- > 0) str.push_back(0);
    while (m--) alp.insert(m);
  }
  std::set<symbol_t> alphabet() const { return alp; }
  state_t initial_state() const { return 0; }
  state_t transition(const state_t s, const symbol_t &a) const {
    const int i = s >> 1;
    if (i >= str.size()) return (i << 1) | 1;
    if (a < str[i]) return ((i + 1) << 1) | 0;
    if (a > str[i]) return ((i + 1) << 1) | 1;
    return s + 2;
  }
  bool is_accept(const state_t s) const { return s == str.size() << 1; }

 private:
  std::vector<symbol_t> str;
  std::set<symbol_t> alp;
};
