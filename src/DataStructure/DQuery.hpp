#pragma once
#include <map>
#include "src/DataStructure/WaveletMatrix.hpp"
class DQuery {
 WaveletMatrix<int> wm;
public:
 template <class T> DQuery(const std::vector<T> &v) {
  std::vector<int> next(v.size(), -1);
  std::map<T, int> mp;
  for (int i= v.size(); i--; mp[v[i]]= i)
   if (mp.count(v[i])) next[mp[v[i]]]= i;
  wm= WaveletMatrix(next);
 }
 std::size_t number_of_types(int l, int r) const { return wm.count(l, r, l); }
};
