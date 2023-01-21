#define PROBLEM "https://atcoder.jp/contests/abc235/tasks/abc235_f"
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Automaton/dfa_dp.hpp"
#include "src/Automaton/dfa_operations.hpp"
#include "src/Automaton/DFA_Inequality.hpp"
using namespace std;
class DFA_Variety {
 int conclude;
public:
 using symbol_t= int;
 DFA_Variety(int c): conclude(c) {}
 inline std::vector<symbol_t> alphabet() const { return {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; }
 inline int initial_state() const { return 0; }
 inline int transition(int s, const symbol_t &a, int) const { return s | ((a || s) << a); }
 inline bool is_accept(int s) const { return (s & conclude) == conclude; }
 inline int state_size() const { return 1 << 10; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 string N;
 cin >> N;
 int M;
 cin >> M;
 int c= 0;
 for (int i= 0; i < M; i++) {
  int C;
  cin >> C, c|= 1 << C;
 }
 using T= pair<Mint, Mint>;
 auto add= [](T &l, const T &r) { l.first+= r.first, l.second+= r.second; };
 auto f= [](const T &v, int a, int) -> T { return {v.first * 10 + v.second * a, v.second}; };
 DFA_Variety dfa_v(c);
 DFA_Inequality dfa_le(N, 10);
 cout << dfa_dp<T>(dfa_v & dfa_le, N.length(), add, f, {0, 0}, {0, 1}).first << '\n';
 return 0;
}