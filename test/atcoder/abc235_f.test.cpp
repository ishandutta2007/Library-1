#define PROBLEM "https://atcoder.jp/contests/abc235/tasks/abc235_f"
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include "src/Misc/Automaton.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 string N;
 int M;
 cin >> N >> M;
 int n= N.length();
 int c= 0;
 for (int i= 0; i < M; i++) {
  int C;
  cin >> C, c|= 1 << C;
 }
 std::vector<int> alp= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
 auto tr0= [&](int s, int q) {
  if (s >= n) return s;
  int d= N[s] - '0';
  if (q > d) return n + 1;
  if (q < d) return n;
  return s + 1;
 };
 auto ac0= [&](int) { return true; };
 Automaton dfa_le(alp, 0, tr0, ac0, n + 1);
 auto tr1= [&](int s, int q) { return s | ((q || s) << q); };
 auto ac1= [&](int s) { return (s & c) == c; };
 Automaton dfa_variety(alp, 0, tr1, ac1);
 auto dfa= dfa_le & dfa_variety;
 using T= array<Mint, 2>;
 auto op= [](const T &l, const T &r) { return T{l[0] + r[0], l[1] + r[1]}; };
 auto f= [](const T &v, int a, int) { return T{v[0], v[1] * 10 + v[0] * a}; };
 cout << dfa.dp_run(n, op, T{0, 0}, f, T{1, 0})[1] << '\n';
 return 0;
}