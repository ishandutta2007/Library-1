#pragma once
#include <bits/stdc++.h>
#include "src/Automaton/dfa_dp.hpp"
/**
 * @title DFAの文字集合の変換と積・補集合演算
 * @category オートマトン
 */

// BEGIN CUT HERE
template <class DFA, class S, class F>
struct DFA_SymbolMap {
  using state_t = typename DFA::state_t;
  using symbol_t = S;
  DFA_SymbolMap(DFA dfa_, std::set<symbol_t> alp_, F f_)
      : dfa(std::move(dfa_)), alp(std::move(alp_)), f(std::move(f_)) {
    static_assert(is_dfa_v<DFA>);
  }
  std::set<symbol_t> alphabet() const { return alp; }
  state_t initial_state() const { return dfa.initial_state(); }
  state_t transition(const state_t &s, const symbol_t &a) const {
    return dfa.transition(s, f(a));
  }
  bool is_accept(const state_t &s) const { return dfa.is_accept(s); }

 private:
  DFA dfa;
  std::set<symbol_t> alp;
  F f;
};

template <class DFA0, class DFA1>
struct DFA_Intersection {
  using state_t = std::pair<typename DFA0::state_t, typename DFA1::state_t>;
  using symbol_t = typename DFA0::symbol_t;
  static_assert(std::is_same_v<symbol_t, typename DFA1::symbol_t>);
  DFA_Intersection(DFA0 dfa0_, DFA1 dfa1_)
      : dfa0(std::move(dfa0_)), dfa1(std::move(dfa1_)) {
    static_assert(is_dfa_v<DFA0>);
    static_assert(is_dfa_v<DFA1>);
  }
  std::set<symbol_t> alphabet() const {
    auto alphabet = dfa0.alphabet();
    assert(alphabet == dfa1.alphabet());
    return alphabet;
  }
  state_t initial_state() const {
    return {dfa0.initial_state(), dfa1.initial_state()};
  }
  state_t transition(const state_t &s, const symbol_t &c) const {
    const auto &[s0, s1] = s;
    return state_t(dfa0.transition(s0, c), dfa1.transition(s1, c));
  }
  bool is_accept(const state_t &s) const {
    const auto &[s0, s1] = s;
    return dfa0.is_accept(s0) && dfa1.is_accept(s1);
  }

 private:
  DFA0 dfa0;
  DFA1 dfa1;
};
template <class DFA0, class DFA1,
          typename std::enable_if_t<is_dfa_v<DFA0> && is_dfa_v<DFA1>,
                                    std::nullptr_t> = nullptr>
DFA_Intersection<DFA0, DFA1> operator&(const DFA0 &dfa0, const DFA1 &dfa1) {
  return DFA_Intersection<DFA0, DFA1>(dfa0, dfa1);
}

template <class DFA>
struct DFA_Complement {
  using state_t = typename DFA::state_t;
  using symbol_t = typename DFA::symbol_t;
  DFA_Complement(DFA dfa_) : dfa(std::move(dfa_)) {
    static_assert(is_dfa_v<DFA>);
  }
  std::set<symbol_t> alphabet() const { return dfa.alphabet(); }
  state_t initial_state() const { return dfa.initial_state(); }
  state_t transition(const state_t &s, const symbol_t &c) const {
    return dfa.transition(s, c);
  }
  bool is_accept(const state_t &s) const { return !dfa.is_accept(s); }

 private:
  DFA dfa;
};
template <class DFA,
          typename std::enable_if_t<is_dfa_v<DFA>, std::nullptr_t> = nullptr>
DFA_Complement<DFA> operator-(const DFA &dfa) {
  return DFA_Complement<DFA>(dfa);
}