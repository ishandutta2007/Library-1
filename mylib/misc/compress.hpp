#pragma once
#include <vector>
#include <algorithm>
template <class T> auto compress(std::vector<T> &v) {
 return std::sort(v.begin(), v.end()), v.erase(std::unique(v.begin(), v.end()), v.end()), [&v](T x) { return std::lower_bound(v.begin(), v.end(), x) - v.begin(); };
}