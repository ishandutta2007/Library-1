#pragma once
#include <bits/stdc++.h>
/**
 * @title $N$以下(以上)の非負整数を受理するDFA
 * @category オートマトン
 * @brief 0-fill
 */

// BEGIN CUT HERE
template <bool ge = false>  // le or ge
struct DFA_Inequality {     // view from the top digit
  using symbol_t = int;
  DFA_Inequality(std::vector<symbol_t> &&str_, std::vector<symbol_t> &&alp_)
      : str(std::move(str_)), alp(std::move(alp_)) {}
  DFA_Inequality(const std::string &n, int m) {  // n : m-ary notation
    assert(2 <= m && m <= 10);
    for (int l = n.length(), i = 0; i < l;) str.push_back(n[i++] - '0');
    for (int i = 0; i < m; i++) alp.push_back(i);
  }
  template <class Int>  // m-ary number
  DFA_Inequality(Int n, int m, int len = 0) {
    for (; n; n /= m, len--) str.push_back(n % m);
    while (len-- > 0) str.push_back(0);
    for (int i = 0; i < m; i++) alp.push_back(i);
    std::reverse(str.begin(), str.end());
  }
  inline std::vector<symbol_t> alphabet() const { return alp; }
  inline int initial_state() const { return 0; }
  inline int transition(int s, const symbol_t &a, int i) const {
    if (s != 0) return s;
    if (i >= (int)str.size()) return -1;
    if (a == str[i]) return 0;
    if constexpr (ge)
      return a < str[i] ? -1 : 1;
    else
      return a > str[i] ? -1 : 1;
  }
  inline bool is_accept(int s) const { return s >= 0; }
  inline int state_size() const { return 2; }

 private:
  std::vector<symbol_t> str, alp;
};
