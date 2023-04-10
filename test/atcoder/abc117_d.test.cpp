#define PROBLEM "https://atcoder.jp/contests/abc117/tasks/abc117_d"
#include <iostream>
#include <vector>
#include "src/Misc/Automaton.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int64_t N, K;
 cin >> N >> K;
 int64_t A[N];
 for (int i= 0; i < N; i++) cin >> A[i];
 vector alp= {0, 1};
 auto tr= [&](int64_t s, int c) { return (s - c + 2) / 2 - 1; };
 auto ac= [&](int64_t) { return true; };
 Automaton dfa(alp, K, tr, ac, int64_t(-1));
 auto op= [](int64_t l, int64_t r) { return max(l, r); };
 auto f= [&](int64_t x, int c, int k) {
  for (int i= N; i--;) x+= (((A[i] >> k) & 1) ^ c) << k;
  return x;
 };
 cout << dfa.dp_run<int64_t>(60, op, int64_t(0), f, int64_t(0)) << '\n';
 return 0;
}