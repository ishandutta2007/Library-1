#define PROBLEM "https://atcoder.jp/contests/abc117/tasks/abc117_d"
#include <iostream>
#include "src/Automaton/dfa_dp.hpp"
#include "src/Automaton/DFA_Inequality.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 long long N, K;
 cin >> N >> K;
 long long A[N];
 for (int i= 0; i < N; i++) cin >> A[i];
 auto add= [](long long &l, const long long &r) {
  if (l < r) l= r;
 };
 auto f= [&](long long x, int b, int k) {
  int h= 40 - k;
  for (int i= N; i--;) x+= (((A[i] >> h) & 1) ^ b) << h;
  return x;
 };
 cout << dfa_dp<long long>(DFA_Inequality(K, 2, 41), 41, add, f, 0, 0) << '\n';
 return 0;
}