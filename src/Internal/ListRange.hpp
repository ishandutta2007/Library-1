#pragma once
#include <vector>
#include <iterator>
template <class T> struct ListRange {
 using Iterator= typename std::vector<T>::iterator;
 Iterator bg, ed;
 Iterator begin() const { return bg; }
 Iterator end() const { return ed; }
 size_t size() const { return std::distance(bg, ed); }
 T &operator[](int i) const { return bg[i]; }
 friend std::ostream &operator<<(std::ostream &os, const ListRange &r) {
  return os << '[' << r.bg[0], std::for_each(r.bg + 1, r.ed, [&os](const T &x) { os << ", " << x; }), os << ']';
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
  return os << '[' << r.bg[0], std::for_each(r.bg + 1, r.ed, [&os](const T &x) { os << ", " << x; }), os << ']';
 }
};