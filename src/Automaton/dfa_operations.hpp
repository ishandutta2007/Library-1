#pragma once
#include <cassert>
#include "src/Automaton/dfa_dp.hpp"
template <class DFA, class S, class F> struct DFA_SymbolMap {
 using symbol_t= S;
 DFA_SymbolMap(const DFA &dfa_, const std::vector<symbol_t> &alp_, const F &f_): dfa(dfa_), alp(alp_), f(f_) { static_assert(is_dfa_v<DFA>); }
 std::vector<symbol_t> alphabet() const { return alp; }
 inline int initial_state() const { return dfa.initial_state(); }
 inline int transition(int s, const symbol_t &a, int i) const { return dfa.transition(s, f(a), i); }
 inline bool is_accept(int s) const { return dfa.is_accept(s); }
 inline int state_size() const { return dfa.state_size(); }
private:
 const DFA dfa;
 const std::vector<symbol_t> alp;
 const F f;
};
template <class DFA0, class DFA1> struct DFA_Intersection {
 using symbol_t= typename DFA0::symbol_t;
 static_assert(std::is_same_v<symbol_t, typename DFA1::symbol_t>);
 DFA_Intersection(const DFA0 &dfa0_, const DFA1 &dfa1_): dfa0(dfa0_), dfa1(dfa1_) {
  static_assert(is_dfa_v<DFA0>);
  static_assert(is_dfa_v<DFA1>);
 }
 inline std::vector<symbol_t> alphabet() const {
  auto alphabet= dfa0.alphabet();
  assert(alphabet == dfa1.alphabet());
  return alphabet;
 }
 inline int initial_state() const { return product(dfa0.initial_state(), dfa1.initial_state()); }
 inline int transition(int s, const symbol_t &c, int i) const {
  auto [s0, s1]= projection(s);
  return product(dfa0.transition(s0, c, i), dfa1.transition(s1, c, i));
 }
 inline bool is_accept(int s) const {
  auto [s0, s1]= projection(s);
  return dfa0.is_accept(s0) && dfa1.is_accept(s1);
 }
 inline int state_size() const { return dfa0.state_size() * dfa1.state_size(); }
private:
 inline int product(int s0, int s1) const { return s0 == -1 || s1 == -1 ? -1 : s0 + s1 * dfa0.state_size(); }
 inline std::pair<int, int> projection(int s) const { return {s % dfa0.state_size(), s / dfa0.state_size()}; }
 const DFA0 dfa0;
 const DFA1 dfa1;
};
template <class DFA0, class DFA1, typename std::enable_if_t<is_dfa_v<DFA0> && is_dfa_v<DFA1>, std::nullptr_t> = nullptr> DFA_Intersection<DFA0, DFA1> operator&(const DFA0 &dfa0, const DFA1 &dfa1) { return DFA_Intersection<DFA0, DFA1>(dfa0, dfa1); }