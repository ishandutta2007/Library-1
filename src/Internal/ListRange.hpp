#pragma once
#include <vector>
#include <iostream>
#include <iterator>
template <class T> struct ListRange {
 using Iterator= typename std::vector<T>::iterator;
 Iterator bg, ed;
 Iterator begin() const { return bg; }
 Iterator end() const { return ed; }
 size_t size() const { return std::distance(bg, ed); }
 T &operator[](int i) const { return bg[i]; }
 friend std::ostream &operator<<(std::ostream &os, const ListRange &r) {
  os << '[';
  for (int i= 0, e= r.size(); i < e; ++i) os << (i ? ", " : "") << r[i];
  return os << ']';
 }
};
template <class T> struct ConstListRange {
 using Iterator= typename std::vector<T>::const_iterator;
 Iterator bg, ed;
 Iterator begin() const { return bg; }
 Iterator end() const { return ed; }
 size_t size() const { return std::distance(bg, ed); }
 const T &operator[](int i) const { return bg[i]; }
 friend std::ostream &operator<<(std::ostream &os, const ConstListRange &r) {
  os << '[';
  for (int i= 0, e= r.size(); i < e; ++i) os << (i ? ", " : "") << r[i];
  return os << ']';
 }
};