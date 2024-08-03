// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc129/tasks/abc129_e
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include "src/Misc/Automaton.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9 + 7)>;
 string L;
 cin >> L;
 int n= L.length();
 using symbol_t= array<bool, 2>;
 vector<symbol_t> alp= {{0, 0}, {0, 1}, {1, 0}};
 auto tr= [&](int s, symbol_t c) {
  if (s >= n) return s;
  bool d= L[s] - '0', e= c[0] ^ c[1];
  if (e > d) return n + 1;
  if (e < d) return n;
  return s + 1;
 };
 auto ac= [&](int s) { return true; };
 Automaton dfa(alp, 0, tr, ac, n + 1);
 cout << dfa.num<Mint>(n) << '\n';
 return 0;
}