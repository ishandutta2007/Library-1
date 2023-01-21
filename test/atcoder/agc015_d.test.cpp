#define PROBLEM "https://atcoder.jp/contests/agc015/tasks/agc015_d"
#include <iostream>
#include <vector>
#include <set>
#include "src/Automaton/dfa_dp.hpp"
#include "src/Automaton/NFA_to_DFA.hpp"
using namespace std;
class AororB {
 const long long A, B;
public:
 using symbol_t= int;
 AororB(long long A_, long long B_): A(A_), B(B_) {}
 std::vector<symbol_t> alphabet() const { return {0, 1}; }
 int initial_state() const { return 3; }
 std::set<int> transition(int s, const symbol_t &c, int i) const {
  if (i >= 60) return {};
  if (s == 0) return {0};
  long long l= A >> (59 - i), u= B >> (59 - i);
  if (l == u) {
   if (c == (l & 1)) return {3};
   return {};
  }
  if (c == 0) {
   if (s == 1) {
    if (l & 1) return {};
    return {1};
   } else if (s == 2 || s == 4) {
    if (u & 1) return {0};
    return {s};
   } else {
    if (l & 1) return {};
    if (u & 1) return {1};
    return {3};
   }
  } else {
   if (s == 1) {
    if (l & 1) return {1};
    return {0};
   } else if (s == 2) {
    if (u & 1) return {4};
    return {};
   } else if (s == 4) {
    if (u & 1) return {0};
    return {4};
   } else {
    if (u - l == 1) {
     if (l & 1) return {3};
     return {3, 2};
    } else {
     if (l & 1) {
      if (u & 1) return {1};
      return {3};
     }
     if (u & 1) return {0};
     return {4};
    }
   }
  }
 }
 std::set<int> eps_transition(const int) const { return {}; }
 bool is_accept(int s) const { return true; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 long long A, B;
 cin >> A >> B;
 cout << dfa_dp<long long>(NFA_to_DFA(AororB(A, B)), 60) << '\n';
 return 0;
}