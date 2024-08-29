// competitive-verifier: PROBLEM https://atcoder.jp/contests/agc021/tasks/agc021_a
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/Misc/Automaton.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 long long N;
 cin >> N;
 vector alp= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
 auto tr= [](long long s, int c) { return (s - c + 10) / 10 - 1; };
 auto ac= [](long long) { return true; };
 Automaton dfa(alp, N, tr, ac, -1ll);
 auto op= [](long long l, long long r) { return max(l, r); };
 auto f= [](long long x, int c, int) { return x + c; };
 cout << dfa.dp_run(60, op, 0ll, f, 0ll) << '\n';
 return 0;
}