#pragma once
#include <vector>
#include <cassert>
#include <cstdint>
template <class K> K div_at(std::vector<K> p, std::vector<K> q, uint64_t k) {
 int n= p.size() - 1, m= q.size() - 1;
 for (assert(q[0] != K(0));; --n)
  if (n < 0 || p[n] != K()) break;
 for (;; --m)
  if (m < 0 || q[m] != K()) break;
 const int l= std::max(n, m) + 1;
 p.resize(l), q.resize(l);
 for (std::vector<K> np; k > m; q.swap(p), p.swap(np), k>>= 1) {
  np.assign(l, K());
  if (k & 1) {
   for (int i= 0; i < l; i+= 2)
    for (int j= 1; j < l; j+= 2) np[(i + j) >> 1]+= p[j] * q[i] - p[i] * q[j];
  } else {
   for (int i= 0; i < l; i+= 2)
    for (int j= 0; j < l; j+= 2) np[(i + j) >> 1]+= p[i] * q[j];
   for (int i= 1; i < l; i+= 2)
    for (int j= 1; j < l; j+= 2) np[(i + j) >> 1]-= p[i] * q[j];
  }
  p.assign(l, K());
  for (int i= 0; i < l; i+= 2)
   for (int j= 0; j < i; j+= 2) p[(i + j) >> 1]+= q[i] * q[j];
  for (int i= 1; i < l; i+= 2)
   for (int j= 1; j < i; j+= 2) p[(i + j) >> 1]-= q[i] * q[j];
  for (int i= l; i--;) p[i]+= p[i];
  for (int i= 0; i < l; i+= 2) p[i]+= q[i] * q[i];
  for (int i= 1; i < l; i+= 2) p[i]-= q[i] * q[i];
 }
 K iv= K(1) / q[0];
 for (int j= 0; j <= k; p[j++]*= iv)
  for (int i= j; i; --i) p[j]-= p[j - i] * q[i];
 return p[k];
}
// a[n] = c[0] * a[n-1] + c[1] * a[n-2] + ... + c[d-1] * a[n-d]
// return a[k]
template <class K> K linear_recurrence(std::vector<K> c, const std::vector<K> &a, uint64_t k) {
 if (k < a.size()) return a[k];
 const size_t d= c.size();
 assert(d <= a.size());
 for (auto &x: c) x= -x;
 std::vector<K> p(d);
 c.insert(c.begin(), K(1));
 for (int i= d; i--;)
  for (int j= i; j >= 0; --j) p[i]+= c[j] * a[i - j];
 return div_at<K>(p, c, k);
}
