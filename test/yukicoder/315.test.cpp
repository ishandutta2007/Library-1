// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/315
// competitive-verifier: TLE 1
// competitive-verifier: MLE 128
#include <iostream>
#include <vector>
#include <string>
#include "src/Math/ModInt.hpp"
#include "src/Misc/Automaton.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9 + 7)>;
 string A, B;
 cin >> A >> B;
 int P;
 cin >> P;
 vector alp= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
 int n_a= A.length(), n_b= B.length();
 for (int i= n_a; i--;) {
  if (A[i] > '0') {
   --A[i];
   break;
  }
  A[i]= '9';
 }
 auto tr_a= [&](int s, int c) {
  if (s >= n_a) return s;
  int d= A[s] - '0';
  if (c < d) return n_a;
  if (c > d) return n_a + 1;
  return s + 1;
 };
 auto tr_b= [&](int s, int c) {
  if (s >= n_b) return s;
  int d= B[s] - '0';
  if (c < d) return n_b;
  if (c > d) return n_b + 1;
  return s + 1;
 };
 auto ac= [&](int) { return true; };
 Automaton dfa_a(alp, 0, tr_a, ac, n_a + 1), dfa_b(alp, 0, tr_b, ac, n_b + 1);
 auto tr_3= [&](int s, int c) {
  if (s == -1 || c == 3) return -1;
  return (s + c) % 3;
 };
 Automaton dfa_3(alp, 0, tr_3, [](int s) { return s <= 0; });
 auto dfa1= dfa_a & dfa_3;
 auto dfa2= dfa_b & dfa_3;
 Mint ans= dfa2.num<Mint>(n_b) - dfa1.num<Mint>(n_a);
 while (P % 10 == 0) P/= 10, --n_a, --n_b;
 Automaton dfa_a2(alp, 0, tr_a, ac, n_a + 1), dfa_b2(alp, 0, tr_b, ac, n_b + 1);
 auto tr_P= [&](int s, int c) { return (s * 10 + c) % P; };
 Automaton dfa_P(alp, 0, tr_P, [](int s) { return s == 0; });
 auto dfa3= dfa_a2 & dfa_3 & dfa_P;
 auto dfa4= dfa_b2 & dfa_3 & dfa_P;
 ans-= dfa4.num<Mint>(n_b) - dfa3.num<Mint>(n_a);
 cout << ans << '\n';
 return 0;
}