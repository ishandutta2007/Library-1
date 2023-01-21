#pragma once
#include <vector>
#include "src/Internal/HAS_CHECK.hpp"
HAS_TYPE(symbol_t);
HAS_MEMBER(alphabet);
HAS_MEMBER(initial_state);
HAS_MEMBER(transition);
HAS_MEMBER(is_accept);
HAS_MEMBER(state_size);
HAS_MEMBER(eps_transition);
HAS_MEMBER(is_reject);
template <class A> using is_automaton= std::conjunction<has_symbol_t<A>, has_alphabet<A>, has_initial_state<A>, has_transition<A>, has_is_accept<A>>;
template <class A> using trans_t= std::invoke_result_t<decltype(&A::transition), A, int, typename A::symbol_t, int>;
template <class DFA> constexpr bool is_dfa_v= std::conjunction_v<has_state_size<DFA>, is_automaton<DFA>, std::is_same<trans_t<DFA>, int>>;
template <class T, class DFA, class Add, class F> T dfa_dp(const DFA &dfa, int len, const Add &add, const F &f, const T t0= T(0), const T init= T(1)) {
 static_assert(is_dfa_v<DFA>);
 const auto alphabet= dfa.alphabet();
 const int S= dfa.state_size();
 std::vector<T> dp(S, t0);
 std::vector<char> visit(S, false);
 dp[dfa.initial_state()]= init, visit[dfa.initial_state()]= true;
 for (int i= 0; i < len; i++) {
  std::vector<T> next(S, t0);
  std::vector<char> next_visit(S, false);
  for (int s= S; s--;)
   if (visit[s])
    for (const auto &a: alphabet)
     if (int q= dfa.transition(s, a, i); q != -1) add(next[q], f(dp[s], a, i)), next_visit[q]= true;
  dp.swap(next), visit.swap(next_visit);
 }
 T ret= t0;
 for (int s= S; s--;)
  if (dfa.is_accept(s)) add(ret, dp[s]);
 return ret;
}
template <class T, class DFA> T dfa_dp(const DFA &dfa, int len, const T t0= T(0), const T init= T(1)) {
 return dfa_dp<T>(
     dfa, len, [](T &l, const T &r) { l+= r; }, [](const T &v, const typename DFA::symbol_t &, int) { return v; }, t0, init);
}