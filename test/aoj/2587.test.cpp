#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/challenges/sources/JAG/Prelim/2587"
#include <bits/stdc++.h>

#include "src/Automaton/dfa_dp.hpp"
#include "src/Automaton/NFA_to_DFA.hpp"
using namespace std;

struct Elevator {
  using state_t = int;
  using symbol_t = int;
  Elevator(int N_, const vector<int> &l, const vector<int> &h)
      : N(N_), low(l), high(h) {}
  std::set<symbol_t> alphabet() const { return {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; }
  state_t initial_state() const { return 0; }
  std::set<state_t> transition(const state_t s, const symbol_t &c) const {
    int i = s / 5, j = s % 5;
    if (i == N) return {};
    if (j == 1) {
      if ((low[i] % 10) <= c) return {5 * (i + 1)};
    } else if (j == 2) {
      return {5 * (i + 1)};
    } else if (j == 3) {
      if (c <= (high[i] % 10)) return {5 * (i + 1)};
    } else if (j == 4) {
      if ((low[i] % 10) <= c && c <= (high[i] % 10)) return {5 * (i + 1)};
    } else {
      if (c == 0) return {};
      int l = low[i] / 10, h = high[i] / 10;
      if (l == h) {
        if (c == l) return {5 * i + 4};
      } else {
        if (c == l) return {5 * i + 1};
        if (l < c && c < h) return {5 * i + 2};
        if (c == h) return {5 * i + 3};
      }
    }
    return {};
  }
  std::set<state_t> eps_transition(const state_t s) const {
    int i = s / 5, j = s % 5;
    if (j == 0 && low[i] / 10 == 0) {
      if (high[i] / 10 == 0) return {5 * i + 4};
      return {5 * i + 1};
    }
    return {};
  }
  bool is_accept(const state_t s) const { return s == 5 * N; }

 private:
  const int N;
  vector<int> low, high;
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  int N;
  vector<int> low, high;
  while (cin >> N && N != 0) {
    low.resize(N), high.resize(N);
    for (int i = 0; i < N; i++) cin >> low[i] >> high[i];
    auto dfa = NFA_to_DFA(Elevator(N, low, high));
    long long ans = 0;
    for (int i = N; i <= 2 * N; i++) ans += dfa_dp<long long>(dfa, i);
    cout << ans << '\n';
  }
  return 0;
}