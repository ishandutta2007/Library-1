#pragma once
#include <bits/stdc++.h>
/**
 * @title DFA上のDP
 * @category オートマトン
 * @see https://kuretchi.github.io/blog/entries/automaton-dp/
 */
// verify用:
// https://atcoder.jp/contests/tdpc/tasks/tdpc_number
// https://atcoder.jp/contests/abc029/tasks/abc029_d
// https://atcoder.jp/contests/donuts-live2014/tasks/donuts_live2014_2
// https://atcoder.jp/contests/joi2012yo/tasks/joi2012yo_f

// BEGIN CUT HERE
#ifndef HAS_CHECK
#define HAS_CHECK(member, Dummy)                              \
  template <class T>                                          \
  struct has_##member {                                       \
    template <class U, Dummy>                                 \
    static std::true_type check(U *);                         \
    static std::false_type check(...);                        \
    static T *mClass;                                         \
    static const bool value = decltype(check(mClass))::value; \
  };
#define HAS_MEMBER(member) HAS_CHECK(member, int dummy = (&U::member, 0))
#define HAS_TYPE(member) HAS_CHECK(member, class dummy = typename U::member)
#endif
HAS_TYPE(state_t);
HAS_TYPE(symbol_t);
HAS_MEMBER(alphabet);
HAS_MEMBER(initial_state);
HAS_MEMBER(transition);
HAS_MEMBER(is_accept);
HAS_MEMBER(eps_transition);
template <class A>
using is_automaton =
    std::conjunction<has_state_t<A>, has_symbol_t<A>, has_alphabet<A>,
                     has_initial_state<A>, has_transition<A>, has_is_accept<A>>;
template <class A>
using trans_t = std::invoke_result_t<decltype(&A::transition), A,
                                     typename A::state_t, typename A::symbol_t>;
template <class DFA>
constexpr bool is_dfa_v =
    std::conjunction_v<is_automaton<DFA>,
                       std::is_same<trans_t<DFA>, typename DFA::state_t>>;
template <class NFA>
constexpr bool is_nfa_v = std::conjunction_v<
    is_automaton<NFA>, has_eps_transition<NFA>,
    std::is_same<trans_t<NFA>, std::set<typename NFA::state_t>>>;

template <class T, class DFA, class Add, class F>
T dfa_dp(const DFA &dfa, int len, const Add &add, const F &f, const T t0 = T(0),
         const T init = T(1)) {
  static_assert(is_dfa_v<DFA>);
  const auto alphabet = dfa.alphabet();
  std::map<typename DFA::state_t, T> dp;
  dp[dfa.initial_state()] = init;
  for (int i = 0; i < len; i++) {
    std::map<typename DFA::state_t, T> next;
    for (const auto &[k, v] : dp)
      for (const auto &a : alphabet) {
        if (auto q = dfa.transition(k, a); next.count(q))
          add(next[q], f(v, a, i));
        else
          next[q] = f(v, a, i);
      }
    dp = std::move(next);
  }
  T ret = t0;
  for (const auto &[k, v] : dp)
    if (dfa.is_accept(k)) add(ret, v);
  return ret;
}
template <class T, class DFA>
T dfa_dp(const DFA &dfa, int len, const T t0 = T(0), const T init = T(1)) {
  return dfa_dp<T>(
      dfa, len, [](T &l, const T &r) { l += r; },
      [](const T &v, const typename DFA::symbol_t &, int) { return v; }, t0,
      init);
}