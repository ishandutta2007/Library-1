// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Final/0401
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/Game/ImpartialGame.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Game= pair<int, int>;
 auto f= [&](const Game &g) {
  vector<Game> ret;
  if (g.first > 0) ret.emplace_back(g), ret.back().first--;
  for (int i= min(g.first, g.second); i > 0; i--) ret.emplace_back(g), ret.back().second-= i;
  if (g.second > 0) ret.emplace_back(g), ret.back().first++, ret.back().second--;
  return ret;
 };
 ImpartialGame<Game, decltype(f)> ig(f);
 int sum= 0, N;
 cin >> N;
 while (N--) {
  int w, b;
  cin >> w >> b;
  sum^= ig.eval(Game(w, b));
 }
 cout << !sum << '\n';
 return 0;
}
