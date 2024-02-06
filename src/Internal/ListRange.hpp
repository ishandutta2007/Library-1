#pragma once
#include <vector>
#include <iterator>
template <class T> struct ListRange {
 using Iterator= typename std::vector<T>::const_iterator;
 Iterator bg, ed;
 Iterator begin() const { return bg; }
 Iterator end() const { return ed; }
 size_t size() const { return std::distance(bg, ed); }
 const T &operator[](int i) const { return bg[i]; }
};