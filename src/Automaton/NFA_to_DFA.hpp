#pragma once
#include <set>
#include <map>
#include <tuple>
#include <algorithm>
#include "src/Automaton/dfa_dp.hpp"
template <class NFA> constexpr bool is_nfa_v= std::conjunction_v<is_automaton<NFA>, has_eps_transition<NFA>, std::is_same<trans_t<NFA>, std::set<int>>>;
template <class NFA> struct NFA_to_DFA {
 using symbol_t= typename NFA::symbol_t;
 NFA_to_DFA(NFA &&nfa_): size(0), nfa(std::move(nfa_)) {
  static_assert(is_nfa_v<NFA>);
  std::set<int> ss{initial_state_()};
  for (int i= 0; !ss.empty(); i++) {
   std::set<int> ts;
   for (int s: ss)
    for (const auto &a: alphabet()) {
     int q= transition_(s, a, i);
     memo[std::make_tuple(s, a, i)]= q;
     if (q != -1) ts.insert(q);
    }
   ss.swap(ts);
  }
 }
 std::vector<symbol_t> alphabet() const { return nfa.alphabet(); }
 inline int initial_state() const { return 0; }
 inline int transition(int s, const symbol_t &c, int i) const { return memo.at(std::make_tuple(s, c, i)); }
 inline bool is_accept(int s) const {
  std::set<int> ss= states[s];
  return std::any_of(ss.begin(), ss.end(), [&](int x) { return nfa.is_accept(x); });
 }
 inline int state_size() const { return size; }
private:
 int size;
 NFA nfa;
 std::vector<std::set<int>> states;
 std::map<std::set<int>, int> mp;
 std::map<std::tuple<int, symbol_t, int>, int> memo;
 inline int mapping(const std::set<int> &ss) {
  if (ss.empty()) return -1;
  if (mp.count(ss)) return mp[ss];
  return states.push_back(ss), mp[ss]= size++;
 }
 inline int transition_(int s, const symbol_t &c, int i) {
  std::set<int> ss;
  for (const auto &x: states[s]) {
   auto ys= nfa.transition(x, c, i);
   ss.insert(ys.begin(), ys.end());
  }
  return eps_closure(ss);
 }
 inline int initial_state_() { return eps_closure({nfa.initial_state()}); }
 inline int eps_closure(std::set<int> ss) {
  for (std::set<int> ts; ss != ts;) {
   ts= ss;
   for (const auto &x: ts) {
    auto ys= nfa.eps_transition(x);
    ss.insert(ys.begin(), ys.end());
   }
  }
  return mapping(ss);
 }
};