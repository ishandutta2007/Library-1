#pragma once
#include "src/Internal/ListRange.hpp"
template <class T> class CsrArray {
 std::vector<T> csr;
 std::vector<int> pos;
public:
 CsrArray()= default;
 CsrArray(const std::vector<T> &c, const std::vector<int> &p): csr(c), pos(p) {}
 size_t size() const { return pos.size() - 1; }
 const ListRange<T> operator[](int i) const { return {csr.cbegin() + pos[i], csr.cbegin() + pos[i + 1]}; }
};