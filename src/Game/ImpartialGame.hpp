#pragma once
#include <bits/stdc++.h>
template <typename Game, typename F> struct ImpartialGame {
 std::map<Game, unsigned> mp;
 F f;  // : Game -> std::vector<Game>
 static unsigned mex(std::vector<unsigned> S) {
  std::sort(S.begin(), S.end());
  S.erase(std::unique(S.begin(), S.end()), S.end());
  for (unsigned i= 0; i < S.size(); i++)
   if (S[i] != i) return i;
  return S.size();
 }
public:
 ImpartialGame(const F &_f): f(_f) {}
 unsigned eval(Game g) {
  if (mp.count(g)) return mp[g];
  std::vector<unsigned> S;
  for (const auto &_g: f(g)) S.emplace_back(eval(_g));
  return mp[g]= mex(S);
 }
};
