#pragma once
#include <bits/stdc++.h>
#include "src/Automaton/dfa_dp.hpp"
template <class DFAx, class S= typename DFAx::state_t> struct DFA_Compress {
 using symbol_t= typename DFAx::symbol_t;
 DFA_Compress(const DFAx &dfa_, int N): size(0), dfa(dfa_) {
  static_assert(is_automaton<DFAx>::value);
  static_assert(has_is_reject<DFAx>::value);
  std::set<int> ss{mapping(dfa.initial_state())};
  for (int i= 0; i < N && !ss.empty(); i++) {
   std::set<int> ts;
   for (int s: ss)
    for (const auto &a: alphabet()) {
     int q= mapping(dfa.transition(states[s], a, i));
     memo[std::make_tuple(s, a, i)]= q;
     if (q != -1) ts.insert(q);
    }
   ss.swap(ts);
  }
 }
 inline std::vector<symbol_t> alphabet() const { return dfa.alphabet(); }
 inline int initial_state() const { return 0; }
 inline int transition(int s, const symbol_t &c, int i) const { return memo.at(std::make_tuple(s, c, i)); }
 inline bool is_accept(int s) const { return dfa.is_accept(states[s]); }
 inline int state_size() const { return size; }
private:
 int size;
 DFAx dfa;
 std::vector<S> states;
 std::map<S, int> mp;
 std::map<std::tuple<int, symbol_t, int>, int> memo;
 inline int mapping(const S &ss) {
  if (dfa.is_reject(ss)) return -1;
  if (mp.count(ss)) return mp[ss];
  return states.push_back(ss), mp[ss]= size++;
 }
};