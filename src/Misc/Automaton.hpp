#pragma once
#include <type_traits>
#include <set>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <queue>
#include <cstdlib>
template <class symbol_t> class Automaton {
 std::vector<int> table;
 std::vector<int8_t> info;
 std::vector<symbol_t> alph;
 const int m;
 template <template <class, class> class Map, class state_t, class F, class G, class H> void build(const state_t &initial_state, const F &transition, const G &is_accept, const H &abs_reject) {
  static_assert(std::is_same_v<bool, std::invoke_result_t<G, state_t>>);
  static_assert(std::is_same_v<bool, std::invoke_result_t<H, state_t>>);
  Map<state_t, int> encode;
  std::vector<state_t> decode;
  int ts= 0;
  decode.push_back(initial_state), encode.emplace(initial_state, ts++);
  for (int i= 0, k= 0; i < ts; ++i) {
   auto s= decode[i];
   table.resize(table.size() + m);
   for (int j= 0; j < m; ++j) {
    if (auto t= transition(s, j); abs_reject(t)) table[k++]= -1;
    else if (auto it= encode.find(t); it != encode.end()) table[k++]= it->second;
    else table[k++]= ts, decode.push_back(t), encode.emplace(t, ts++);
   }
  }
  info.resize(ts);
  for (int i= ts; i--;) info[i]= is_accept(decode[i]);
 }
 Automaton(const std::vector<symbol_t> &alphabet): alph(alphabet), m(alph.size()) {}
public:
 template <class state_t, class F, class G, std::enable_if_t<std::is_same_v<state_t, std::invoke_result_t<F, state_t, symbol_t>>, std::nullptr_t> = nullptr> Automaton(const std::vector<symbol_t> &alphabet, const state_t &initial_state, const F &transition, const G &is_accept): alph(alphabet), m(alph.size()) {
  std::sort(alph.begin(), alph.end());
  auto tr= [&](const state_t &s, int i) { return transition(s, alph[i]); };
  auto rej= [](const state_t &) { return false; };
  if constexpr (std::is_integral_v<state_t>) build<std::unordered_map, state_t, decltype(tr), G, decltype(rej)>(initial_state, tr, is_accept, rej);
  else build<std::map, state_t, decltype(tr), G, decltype(rej)>(initial_state, tr, is_accept, rej);
 }
 template <class state_t, class F, class G, std::enable_if_t<std::is_same_v<state_t, std::invoke_result_t<F, state_t, symbol_t>>, std::nullptr_t> = nullptr> Automaton(const std::vector<symbol_t> &alphabet, const state_t &initial_state, const F &transition, const G &is_accept, const state_t &abs_rej_state): alph(alphabet), m(alph.size()) {
  std::sort(alph.begin(), alph.end());
  auto tr= [&](const state_t &s, int i) { return transition(s, alph[i]); };
  auto rej= [abs_rej_state](const state_t &s) { return s == abs_rej_state; };
  if constexpr (std::is_integral_v<state_t>) build<std::unordered_map, state_t, decltype(tr), G, decltype(rej)>(initial_state, tr, is_accept, rej);
  else build<std::map, state_t, decltype(tr), G, decltype(rej)>(initial_state, tr, is_accept, rej);
 }
 template <class state_t, class F, class G, std::enable_if_t<std::is_same_v<std::set<state_t>, std::invoke_result_t<F, state_t, symbol_t>>, std::nullptr_t> = nullptr> Automaton(const std::vector<symbol_t> &alphabet, const state_t &initial_state, const F &transition, const G &is_accept): alph(alphabet), m(alph.size()) {
  static_assert(std::is_same_v<bool, std::invoke_result_t<G, state_t>>);
  std::sort(alph.begin(), alph.end());
  auto tr= [&](const std::set<state_t> &s, int i) {
   std::set<state_t> ret;
   for (const auto &x: s) {
    auto ys= transition(x, alph[i]);
    ret.insert(ys.begin(), ys.end());
   }
   return ret;
  };
  auto ac= [&](const std::set<state_t> &s) { return std::any_of(s.begin(), s.end(), is_accept); };
  auto rej= [](const std::set<state_t> &s) { return s == std::set<state_t>(); };
  build<std::map, std::set<state_t>, decltype(tr), decltype(ac), decltype(rej)>(std::set<state_t>({initial_state}), tr, ac, rej);
 }
 template <class state_t, class F, class G, class H, std::enable_if_t<std::is_same_v<std::set<state_t>, std::invoke_result_t<F, state_t, symbol_t>>, std::nullptr_t> = nullptr> Automaton(const std::vector<symbol_t> &alphabet, const state_t &initial_state, const F &transition, const G &is_accept, const H &eps_trans): alph(alphabet), m(alph.size()) {
  static_assert(std::is_same_v<bool, std::invoke_result_t<G, state_t>>);
  static_assert(std::is_same_v<std::set<state_t>, std::invoke_result_t<H, state_t>>);
  std::sort(alph.begin(), alph.end());
  auto eps_closure= [&](std::set<state_t> s) {
   for (std::set<state_t> t; s != t;) {
    t= s;
    for (const auto &x: t) {
     auto ys= eps_trans(x);
     s.insert(ys.begin(), ys.end());
    }
   }
   return s;
  };
  auto tr= [&](const std::set<state_t> &s, int i) {
   std::set<state_t> ret;
   for (const auto &x: s) {
    auto ys= transition(x, alph[i]);
    ret.insert(ys.begin(), ys.end());
   }
   return eps_closure(ret);
  };
  auto ac= [&](const std::set<state_t> &s) { return std::any_of(s.begin(), s.end(), is_accept); };
  auto rej= [](const std::set<state_t> &s) { return s == std::set<state_t>(); };
  build<std::map, std::set<state_t>, decltype(tr), decltype(ac), decltype(rej)>(eps_closure({initial_state}), tr, ac, rej);
 }
 size_t alphabet_size() const { return m; }
 Automaton operator&(const Automaton &r) const {
  assert(alph == r.alph);
  const int S= info.size();
  auto tr= [&](int s, int q) {
   auto [s1, s0]= std::div(s, S);
   int t1= r.table[s1 * m + q], t0= table[s0 * m + q];
   return t0 == -1 || t1 == -1 ? -1 : t1 * S + t0;
  };
  auto ac= [&](int s) {
   auto [s1, s0]= std::div(s, S);
   return info[s0] == 1 && r.info[s1] == 1;
  };
  auto rej= [](int s) { return s == -1; };
  Automaton ret(alph);
  return ret.build<std::unordered_map, int, decltype(tr), decltype(ac), decltype(rej)>(0, tr, ac, rej), ret;
 }
 template <class T, class A, class F> T dp_run(int n, const A &op, const T &ti, const F &f, const T &init) const {
  static_assert(std::is_same_v<T, std::invoke_result_t<A, T, T>>);
  static_assert(std::is_same_v<T, std::invoke_result_t<F, T, symbol_t, int>>);
  const size_t S= info.size();
  std::queue<std::pair<int, int>> que;
  T dp[2][S], ret= ti;
  bool in[2][S];
  for (std::fill_n(dp[0], S, ti), std::fill_n(dp[1], S, ti), std::fill_n(in[0], S, 0), std::fill_n(in[1], S, 0), dp[0][0]= init, que.emplace(0, 0), in[0][0]= 1; que.size();) {
   auto [s, i]= que.front();
   bool b= i & 1;
   T tmp= dp[b][s];
   if (que.pop(), in[b][s]= 0, dp[b][s]= ti; i == n) {
    if (info[s] == 1) ret= op(ret, tmp);
    continue;
   }
   auto l= table.cbegin() + s * m;
   for (int j= m; j--;)
    if (int t= l[j]; t != -1)
     if (dp[!b][t]= op(dp[!b][t], f(tmp, alph[j], i)); !in[!b][t]) que.emplace(t, i + 1), in[!b][t]= 1;
  }
  return ret;
 }
 template <class T> T num(int n) const {
  return dp_run(
      n, [](const T &l, const T &r) { return l + r; }, T(), [](const T &x, const auto &, auto) { return x; }, T(1));
 }
};