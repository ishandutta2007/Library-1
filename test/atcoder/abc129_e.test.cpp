#define PROBLEM "https://atcoder.jp/contests/abc129/tasks/abc129_e"
#include <iostream>
#include "src/Automaton/dfa_dp.hpp"
#include "src/Automaton/DFA_Inequality.hpp"
#include "src/Automaton/dfa_operations.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9 + 7)>;
 string L;
 cin >> L;
 vector<tuple<int, int, int>> alphabet{{0, 0, 0}, {0, 1, 1}, {1, 0, 1}};
 auto dfa_le= DFA_SymbolMap(DFA_Inequality(L, 2), alphabet, [](const tuple<int, int, int> &a) { return get<2>(a); });
 cout << dfa_dp<Mint>(dfa_le, L.length()) << '\n';
 return 0;
}