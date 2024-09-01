// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc155/tasks/abc155_e
// competitive-verifier: TLE 2
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include "src/Misc/Automaton.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 vector<int> alp(19);
 for (int i= 0; i < 19; ++i) alp[i]= i - 9;
 string N;
 cin >> N;
 N= "0" + N;
 int n= N.length();
 using state_t= array<int, 2>;
 auto tr= [&](state_t s, int c) -> state_t {
  auto [i, b]= s;
  if (i < 0) return {-1, -1};
  int d= (N[i] - '0' + b) % 10;
  if (c == d || 10 + c == d) return {i - 1, (N[i] - '0' + b - c) / 10};
  return {-1, -1};
 };
 auto ac= [&](state_t s) { return s[1] == 0; };
 Automaton dfa(alp, state_t{n - 1, 0}, tr, ac, state_t{-1, -1});
 auto op= [&](int l, int r) { return min(l, r); };
 auto f= [&](int x, int c, int) { return x + abs(c); };
 cout << dfa.dp_run(n, op, 1 << 30, f, 0) << '\n';
 return 0;
}