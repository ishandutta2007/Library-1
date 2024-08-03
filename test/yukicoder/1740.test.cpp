// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1740
#include <iostream>
#include <vector>
#include <string>
#include "src/Misc/Automaton.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 vector<char> alp(26);
 for (int i= 0; i < 26; ++i) alp[i]= 'a' + i;
 int N;
 cin >> N;
 string S;
 cin >> S;
 auto tr_le= [&](int s, char c) {
  if (s >= N) return s;
  if (c < S[s]) return N + 1;
  if (c > S[s]) return N;
  return s + 1;
 };
 Automaton dfa_le(
     alp, 0, tr_le, [&](int) { return true; }, N);
 auto tr_a= [&](int s, char c) {
  if (c == 'a') ++s;
  return s;
 };
 Automaton dfa_1(
     alp, 0, tr_a, [](int s) { return s == 1; }, 2);
 auto dfa= dfa_le & dfa_1;
 cout << dfa.num<Mint>(N) << '\n';
 return 0;
}