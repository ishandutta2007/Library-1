#pragma once
#include <bits/stdc++.h>
#include "src/Automaton/dfa_dp.hpp"
/**
 * @title NFAをDFAに変換
 * @category オートマトン
 * @see https://qiita.com/hotman78/items/74c2d5d246ba3b6b3836
 * @see https://blog.knshnb.com/posts/aoj2587/
 */
// verify用
// https://atcoder.jp/contests/abc050/tasks/arc066_b

// BEGIN CUT HERE

template <class NFA>
struct NFA_to_DFA {
  using state_t = std::set<typename NFA::state_t>;
  using symbol_t = typename NFA::symbol_t;
  NFA_to_DFA(NFA nfa_) : nfa(std::move(nfa_)) { static_assert(is_nfa_v<NFA>); }
  std::set<symbol_t> alphabet() const { return nfa.alphabet(); }
  state_t initial_state() const { return eps_closure({nfa.initial_state()}); }
  state_t transition(const state_t &s, const symbol_t &c) const {
    state_t t;
    for (const auto &x : s) {
      auto ys = nfa.transition(x, c);
      t.insert(ys.begin(), ys.end());
    }
    return eps_closure(t);
  }
  bool is_accept(const state_t &s) const {
    return std::any_of(s.begin(), s.end(),
                       [&](const auto &x) { return nfa.is_accept(x); });
  }

 private:
  NFA nfa;
  state_t eps_closure(state_t s) const {
    for (state_t t; s != t;) {
      t = s;
      for (const auto &x : t) {
        auto ys = nfa.eps_transition(x);
        s.insert(ys.begin(), ys.end());
      }
    }
    return s;
  }
};