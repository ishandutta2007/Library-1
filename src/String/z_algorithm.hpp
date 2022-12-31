#pragma once
#include <bits/stdc++.h>
std::vector<int> z_algorithm(const std::string &s) {
 std::vector<int> prefix(s.size());
 for (int i= 1, j= 0; i < s.size(); i++) {
  if (i + prefix[i - j] >= j + prefix[j]) {
   int k= std::max(0, j + prefix[j] - i);
   while (i + k < s.size() && s[k] == s[i + k]) ++k;
   prefix[i]= k, j= i;
  } else prefix[i]= prefix[i - j];
 }
 return prefix[0]= (int)s.size(), prefix;
}