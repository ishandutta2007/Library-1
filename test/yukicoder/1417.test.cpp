#define PROBLEM "https://yukicoder.me/problems/no/1417"
#include <iostream>
#include <vector>
#include <string>
#include "src/Math/ModInt.hpp"
#include "src/Misc/Automaton.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 vector alp= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
 string N;
 cin >> N;
 int n= N.length();
 auto tr_le= [&](int s, int c) {
  if (s >= n) return s;
  int d= N[s] - '0';
  if (c < d) return n;
  if (c > d) return n + 1;
  return s + 1;
 };
 Automaton dfa_le(
     alp, 0, tr_le, [&](int) { return true; }, n + 1);
 auto tr_prod= [](int s, int c) {
  if (s == -1) return c ? c : -1;
  if (!c) return -2;
  return s * c % 100;
 };
 Automaton dfa_prod(
     alp, -1, tr_prod, [](int s) { return s == 0; }, -2);
 auto dfa= dfa_le & dfa_prod;
 cout << dfa.num<Mint>(n) << '\n';
 return 0;
}