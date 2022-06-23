#define PROBLEM "https://atcoder.jp/contests/agc015/tasks/agc015_d"
#include <bits/stdc++.h>
#include "src/Automaton/dfa_dp.hpp"
#include "src/Automaton/NFA_to_DFA.hpp"
using namespace std;

struct AororB {
  using state_t = std::pair<int, int>;
  using symbol_t = int;
  AororB(long long A_, long long B_) : A(A_), B(B_) {}
  std::set<symbol_t> alphabet() const { return {0, 1}; }
  state_t initial_state() const { return {3, 59}; }
  std::set<state_t> transition(const state_t s, const symbol_t &c) const {
    const auto &[v, h] = s;
    if (v == 0) return {{0, -1}};
    long long l = A >> h, u = B >> h;
    if (l == u) {
      if (c == (l & 1)) return {{3, h - 1}};
      return {};
    }
    if (c == 0) {
      if (v == 1) {
        if (l & 1) return {};
        return {{1, h - 1}};
      } else if (v == 2 || v == 4) {
        if (u & 1) return {{0, -1}};
        return {{v, h - 1}};
      } else {
        if (l & 1) return {};
        if (u & 1) return {{1, h - 1}};
        return {{3, h - 1}};
      }
    } else {
      if (v == 1) {
        if (l & 1) return {{1, h - 1}};
        return {{0, -1}};
      } else if (v == 2) {
        if (u & 1) return {{4, h - 1}};
        return {};
      } else if (v == 4) {
        if (u & 1) return {{0, -1}};
        return {{4, h - 1}};
      } else {
        if (u - l == 1) {
          if (l & 1) return {{3, h - 1}};
          return {{3, h - 1}, {2, h - 1}};
        } else {
          if (l & 1) {
            if (u & 1) return {{1, h - 1}};
            return {{3, h - 1}};
          }
          if (u & 1) return {{0, -1}};
          return {{4, h - 1}};
        }
      }
    }
  }
  std::set<state_t> eps_transition(const state_t) const { return {}; }
  bool is_accept(const state_t s) const { return s.second == -1; }

 private:
  const long long A, B;
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  long long A, B;
  cin >> A >> B;
  cout << dfa_dp<long long>(NFA_to_DFA(AororB(A, B)), 60) << '\n';
  return 0;
}