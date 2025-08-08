// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/arc127/tasks/arc127_a
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <array>
#include "src/Misc/Automaton.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int64_t N;
 cin >> N;
 vector alp= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
 auto tr= [](int64_t s, int c) { return (s - c + 10) / 10 - 1; };
 auto ac= [](int64_t) { return true; };
 Automaton dfa(alp, N, tr, ac, int64_t(-1));
 using T= array<int64_t, 2>;
 auto op= [](const T& l, const T& r) { return T{l[0] + r[0], l[1] + r[1]}; };
 auto f= [](T x, int c, int) {
  if (c == 1) x[1]+= x[0];
  else x[1]= 0;
  return x;
 };
 int64_t ans= 0;
 for (int i= 1; i <= 60; ++i) ans+= dfa.dp_run(i, op, T{0, 0}, f, T{1, 0})[1];
 cout << ans << '\n';
 return 0;
}