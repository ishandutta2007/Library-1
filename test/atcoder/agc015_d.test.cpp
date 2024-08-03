// competitive-verifier: PROBLEM https://atcoder.jp/contests/agc015/tasks/agc015_d
#include <iostream>
#include <vector>
#include <set>
#include "src/Misc/Automaton.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 vector alphabet= {0, 1};
 using state_t= set<pair<int64_t, int64_t>>;
 auto tr= [](const state_t &S, int c) -> set<state_t> {
  set<state_t> ret;
  if (c == 0) {
   state_t T;
   bool isok= true;
   for (auto [a, b]: S) {
    if (a= (a + 1) / 2, b/= 2; a > b) {
     isok= false;
     break;
    }
    T.emplace(a, b);
   }
   if (isok) ret.insert(T);
  } else {
   int sz= S.size();
   for (int s= 1 << sz; --s;)
    for (int t= s;; --t&= s) {
     state_t T;
     bool isok= true;
     auto it= S.cbegin();
     for (int j= sz; j--; ++it) {
      auto [a, b]= *it;
      auto a0= (a + 1) / 2, b0= b / 2;
      auto a1= a / 2, b1= (b + 1) / 2 - 1;
      if ((t >> j) & 1) {
       if (a0 > b0 || a1 > b1) {
        isok= false;
        break;
       }
       T.emplace(a0, b0), T.emplace(a1, b1);
      } else if ((s >> j) & 1) {
       if (a1 > b1) {
        isok= false;
        break;
       }
       T.emplace(a1, b1);
      } else {
       if (a0 > b0) {
        isok= false;
        break;
       }
       T.emplace(a0, b0);
      }
     }
     if (isok) ret.insert(T);
     if (!t) break;
    }
  }
  return ret;
 };
 int64_t A, B;
 cin >> A >> B;
 Automaton nfa(alphabet, state_t({{A, B}}), tr, [](const auto &) { return true; });
 cout << nfa.num<int64_t>(60) << '\n';
 return 0;
}