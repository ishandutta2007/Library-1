// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/911
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Misc/Automaton.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 int64_t L, R;
 cin >> L >> R;
 vector<int64_t> A(N);
 for (int i= 0; i < N; ++i) cin >> A[i];
 vector alp= {0, 1};
 auto tr_l= [&](int s, int c) {
  if (s <= -1) return s;
  int d= (L >> s) & 1;
  if (c < d) return -2;
  if (c > d) return -1;
  return s - 1;
 };
 auto tr_r= [&](int s, int c) {
  if (s <= -1) return s;
  int d= (R >> s) & 1;
  if (c < d) return -1;
  if (c > d) return -2;
  return s - 1;
 };
 auto ac0= [&](int) { return true; };
 Automaton dfa_l(alp, 60, tr_l, ac0, -2), dfa_r(alp, 60, tr_r, ac0, -2);
 using state_t= pair<vector<int>, int>;
 state_t rej= make_pair(vector<int>(), -1);
 state_t init= make_pair(vector<int>{0, N}, 60);
 auto tr_x= [&](const state_t &s, int c) -> state_t {
  const auto &[sep, i]= s;
  if (i <= -1) return s;
  const int n= sep.size();
  vector<int> nsep{0};
  for (int j= 0; j + 1 < n; ++j) {
   int bg= sep[j], ed= sep[j + 1];
   for (int k= bg; k + 1 < ed; ++k) {
    int l= ((A[k] >> i) & 1) ^ c, r= ((A[k + 1] >> i) & 1) ^ c;
    if (l > r) return rej;
    if (l < r) nsep.push_back(k + 1);
   }
   nsep.push_back(ed);
  }
  return {nsep, i - 1};
 };
 auto ac_x= [&](const state_t &s) {
  const auto &[sep, i]= s;
  return sep.size() == N + 1;
 };
 Automaton dfa_x(alp, init, tr_x, ac_x, rej);
 auto dfa= dfa_l & dfa_r & dfa_x;
 cout << dfa.num<int64_t>(61) << '\n';
 return 0;
}