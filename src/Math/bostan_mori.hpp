#pragma once
#include <vector>
#include <cassert>
template <class K, bool skip_iv= false> K div_at(std::vector<K> p, std::vector<K> q, uint64_t k) {
 unsigned n= p.size() - 1, m= q.size() - 1, nn, j;
 for (;; --n)
  if (n < 0 || p[n] != K()) break;
 for (;; --m)
  if (m < 0 || q[m] != K()) break;
 const unsigned l= std::max(n, m) + 1;
 std::vector<K> tmp(l);
 for (p.resize(l), q.resize(l); k > m; q.swap(p), p.swap(tmp)) {
  for (tmp.assign((nn= (n + m - ((n ^ m ^ k) & 1)) >> 1) + 1, K()), j= 0; j <= m; j+= 2)
   for (unsigned i= k & 1; i <= n; i+= 2) tmp[(i + j) >> 1]+= p[i] * q[j];
  for (j= 1; j <= m; j+= 2)
   for (unsigned i= (~k) & 1; i <= n; i+= 2) tmp[(i + j) >> 1]-= p[i] * q[j];
  for (p.assign(m + 1, K()), j= 2; j <= m; j+= 2)
   for (int i= j; (i-= 2) >= 0;) p[(i + j) >> 1]+= q[i] * q[j];
  for (k>>= 1, n= nn, j= 3; j <= m; j+= 2)
   for (int i= j; (i-= 2) >= 0;) p[(i + j) >> 1]-= q[i] * q[j];
  for (int i= m + 1; i--;) p[i]+= p[i];
  for (unsigned i= 0; i <= m; i+= 2) p[i]+= q[i] * q[i];
  for (unsigned i= 1; i <= m; i+= 2) p[i]-= q[i] * q[i];
 }
 p.resize(k + 1);
 if constexpr (skip_iv)
  for (j= 0; j <= k; ++j)
   for (int i= j; i; --i) p[j]-= p[j - i] * q[i];
 else
  for (K iv= K(1) / q[j= 0]; j <= k; p[j++]*= iv)
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
 return div_at<K, true>(p, c, k);
}
