// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc135/tasks/abc135_d
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 256
#include <iostream>
#include <vector>
#include "src/Misc/Automaton.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9 + 7)>;
 string S;
 cin >> S;
 int n= S.length();
 vector alp= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
 auto tr_s= [&](int s, int c) {
  if (S[s] == '?' || S[s] == c + '0') return s + 1;
  return -1;
 };
 auto ac_s= [](int) { return true; };
 Automaton dfa_s(alp, 0, tr_s, ac_s, -1);
 auto tr_13= [](int s, int c) { return (s * 10 + c) % 13; };
 auto ac_13= [](int s) { return s == 5; };
 Automaton dfa_13(alp, 0, tr_13, ac_13);
 auto dfa= dfa_s & dfa_13;
 cout << dfa.num<Mint>(n) << '\n';
 return 0;
}