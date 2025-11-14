// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/0570
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include "src/Misc/Automaton.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<10000>;
 string A, B;
 cin >> A >> B;
 int M;
 cin >> M;
 int n= B.length();
 A= string(n - A.length(), '0') + A;
 vector alp= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
 auto tr_a= [&](int s, int c) {
  if (s >= n) return s;
  int d= A[s] - '0';
  if (c < d) return n + 1;
  if (c > d) return n;
  return s + 1;
 };
 auto tr_b= [&](int s, int c) {
  if (s >= n) return s;
  int d= B[s] - '0';
  if (c > d) return n + 1;
  if (c < d) return n;
  return s + 1;
 };
 auto ac= [&](int) { return true; };
 Automaton dfa_a(alp, 0, tr_a, ac, n + 1), dfa_b(alp, 0, tr_b, ac, n + 1);
 auto tr_m= [&](int s, int c) { return (s * 10 + c) % M; };
 Automaton dfa_m(alp, 0, tr_m, [](int s) { return s == 0; });
 using state_t= array<int, 2>;
 auto tr_zz= [&](state_t s, int c) -> state_t {
  auto [d, t]= s;
  if (d == -2) {
   if (c == 0) return {-2, -2};
   else return {c, -2};
  }
  if (d == c) return {-1, -1};
  if (t == -2) return {c, d < c};
  if (d < c) {
   if (t == 1) return {-1, -1};
   else return {c, 1};
  } else {
   if (t == 0) return {-1, -1};
   else return {c, 0};
  }
  return {-1, -1};
 };
 Automaton dfa_zz(alp, state_t{-2, -2}, tr_zz, [&](state_t) { return true; }, state_t{-1, -1});
 auto dfa= dfa_a & dfa_b & dfa_m & dfa_zz;
 cout << dfa.num<Mint>(n) << '\n';
 return 0;
}