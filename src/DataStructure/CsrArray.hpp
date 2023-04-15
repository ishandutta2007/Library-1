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
template <class T> class CsrArray {
 std::vector<T> csr;
 std::vector<int> pos;
public:
 CsrArray()= default;
 CsrArray(const std::vector<T> &c, const std::vector<int> &p): csr(c), pos(p) {}
 size_t size() const { return pos.size() - 1; }
 const ListRange<T> operator[](int i) const { return {csr.cbegin() + pos[i], csr.cbegin() + pos[i + 1]}; }
};