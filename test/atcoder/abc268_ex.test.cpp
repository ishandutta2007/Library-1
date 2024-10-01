// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc268/tasks/abc268_Ex
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/abc268/tasks/abc268_h

#include <iostream>
#include <string>
#include <vector>
#include "src/String/AhoCorasick.hpp"
using namespace std;
int main() {
 ios::sync_with_stdio(0);
 cin.tie(0);
 string S;
 int N;
 cin >> S >> N;
 vector<string> T(N);
 for (int i= 0; i < N; i++) cin >> T[i];
 AhoCorasick ac(T);
 int ans= 0;
 int s= ac.initial_state();
 for (char c: S) {
  s= ac.transition(s, c);
  if (ac.is_accept(s)) {
   ans++;
   s= ac.initial_state();
  }
 }
 cout << ans << '\n';
 return 0;
}
