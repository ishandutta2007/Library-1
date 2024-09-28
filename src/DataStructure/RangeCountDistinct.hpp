#pragma once
#include <map>
#include "src/DataStructure/WaveletMatrix.hpp"
class RangeCountDistinct {
 WaveletMatrix<int> wm;
 template <class T> std::vector<int> build(const std::vector<T> &v) {
  std::vector<int> next(v.size(), -1);
  std::map<T, int> mp;
  for (int i= v.size(); i--; mp[v[i]]= i)
   if (mp.count(v[i])) next[mp[v[i]]]= i;
  return next;
 }
public:
 template <class T> RangeCountDistinct(const std::vector<T> &v): wm(build(v)) {}
 size_t count(int l, int r) const { return wm.count(l, r, l); }
};
