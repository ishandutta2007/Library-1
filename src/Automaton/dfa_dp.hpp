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
HAS_TYPE(symbol_t);
HAS_MEMBER(alphabet);
HAS_MEMBER(initial_state);
HAS_MEMBER(transition);
HAS_MEMBER(is_accept);
HAS_MEMBER(state_size);
HAS_MEMBER(eps_transition);
template <class A>
using is_automaton =
    std::conjunction<has_symbol_t<A>, has_alphabet<A>, has_initial_state<A>,
                     has_transition<A>, has_is_accept<A>>;
template <class A>
using trans_t = std::invoke_result_t<decltype(&A::transition), A, int,
                                     typename A::symbol_t, int>;
template <class DFA>
constexpr bool is_dfa_v =
    std::conjunction_v<has_state_size<DFA>, is_automaton<DFA>,
                       std::is_same<trans_t<DFA>, int>>;
template <class T, class DFA, class Add, class F>
T dfa_dp(const DFA &dfa, int len, const Add &add, const F &f, const T t0 = T(0),
         const T init = T(1)) {
  static_assert(is_dfa_v<DFA>);
  const auto alphabet = dfa.alphabet();
  const int S = dfa.state_size;
  std::vector<T> dp(S, t0);
  std::vector<char> visit(S, false);
  dp[dfa.initial_state()] = init, visit[dfa.initial_state()] = true;
  for (int i = 0; i < len; i++) {
    std::vector<T> next(S, t0);
    std::vector<char> next_visit(S, false);
    for (int s = S; s--;)
      if (visit[s])
        for (const auto &a : alphabet)
          if (int q = dfa.transition(s, a, i); q != -1)
            add(next[q], f(dp[s], a, i)), next_visit[q] = true;
    dp.swap(next), visit.swap(next_visit);
  }
  T ret = t0;
  for (int s = S; s--;)
    if (dfa.is_accept(s)) add(ret, dp[s]);
  return ret;
}
template <class T, class DFA>
T dfa_dp(const DFA &dfa, int len, const T t0 = T(0), const T init = T(1)) {
  return dfa_dp<T>(
      dfa, len, [](T &l, const T &r) { l += r; },
      [](const T &v, const typename DFA::symbol_t &, int) { return v; }, t0,
      init);
}