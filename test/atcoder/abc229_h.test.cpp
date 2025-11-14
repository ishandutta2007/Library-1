// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc229/tasks/abc229_h
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <string>
#include "src/Game/PartisanGame.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 using Game= vector<int>;
 auto f= [&](const Game &g) {
  vector<Game> gs[2];
  for (int b= 0; b < 2; b++)
   for (int i= 0; i < N; i++)
    if (g[i] == !b) gs[b].emplace_back(g), gs[b].back()[i]= 2;
    else if (g[i] == b && i > 0 && g[i - 1] == 2) gs[b].emplace_back(g), gs[b].back()[i - 1]= b, gs[b].back()[i]= 2;
  return make_pair(gs[0], gs[1]);
 };
 PartisanGame<Game, decltype(f)> pg(f);
 string S[N];
 for (int i= 0; i < N; i++) cin >> S[i];
 DyadicRational sum(0);
 for (int j= 0; j < N; j++) {
  Game g(N);
  for (int i= 0; i < N; i++) g[i]= S[i][j] == '.' ? 2 : (S[i][j] == 'B');
  sum+= pg.eval(g);
 }
 cout << (sum > 0 ? "Takahashi" : "Snuke") << '\n';
 return 0;
}