#pragma once
#include <vector>
#include <string>
struct KnuthMorrisPratt {
 std::vector<int> KMP;
 const std::string s;
 int n;
 KnuthMorrisPratt(const std::string &str): s(str), n(s.length()) {
  KMP.resize(n + 1, -1);
  std::vector<int> knuth(n + 1, -1);
  for (int i= 0, j= -1; i < n; i++) {
   while (~j && s[i] != s[j]) j= knuth[j];
   knuth[i + 1]= KMP[i + 1]= ++j;
   if (i + 1 < n && s[i + 1] == s[j]) knuth[i + 1]= knuth[j];
  }
 }
 // the period of S[0:i]
 int period(int i) { return i - KMP[i]; }
 // positions for T that match s
 // O(|T|)
 std::vector<int> pattern_match(std::string T) {
  int m= T.length();
  std::vector<int> occur;
  for (int i= 0, k= 0; i < m; ++i) {
   while (k >= 0 && T[i] != s[k]) k= KMP[k];
   if (++k == n) {
    /* match at T[i-n+1 ... i] */
    occur.push_back(i - n + 1);
   }
  }
  return occur;
 }
};