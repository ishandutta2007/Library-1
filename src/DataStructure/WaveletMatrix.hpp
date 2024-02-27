#pragma once
#include <vector>
#include <algorithm>
#include <array>
#include <cassert>
template <class T> class WaveletMatrix {
 struct SuccinctIndexableDictionary {
  int len, blocks, zeros;
  std::vector<unsigned> bit, sum;
  SuccinctIndexableDictionary(int len): len(len), blocks((len >> 5) + 1), bit(blocks, 0), sum(blocks, 0) {}
  void set(int k) { bit[k >> 5]|= 1U << (k & 31); }
  void build() {
   for (int i= 1; i < blocks; ++i) sum[i]= sum[i - 1] + __builtin_popcount(bit[i - 1]);
   zeros= rank0(len);
  }
  bool operator[](int k) const { return (bit[k >> 5] >> (k & 31)) & 1; }
  int rank(int k) const { return (sum[k >> 5] + __builtin_popcount(bit[k >> 5] & ((1U << (k & 31)) - 1))); }
  int rank0(int k) const { return k - rank(k); }
 };
 int len, lg;
 std::vector<SuccinctIndexableDictionary> mat;
 std::vector<T> vec;
public:
 WaveletMatrix(const std::vector<T> &v): len(v.size()), lg(len ? 32 - __builtin_clz(len) : 1), mat(lg, len), vec(v) {
  std::sort(vec.begin(), vec.end()), vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
  std::vector<unsigned> cur(len), nex(len);
  for (int i= len; i--;) cur[i]= std::lower_bound(vec.begin(), vec.end(), v[i]) - vec.begin();
  for (auto h= lg; h--; cur.swap(nex)) {
   for (int i= 0; i < len; ++i)
    if ((cur[i] >> h) & 1) mat[h].set(i);
   mat[h].build();
   std::array it{nex.begin(), nex.begin() + mat[h].zeros};
   for (int i= 0; i < len; ++i) *it[mat[h][i]]++= cur[i];
  }
 }
 // k-th(0-indexed) smallest number in v[l,r)
 T kth_smallest(int l, int r, int k) const {
  assert(k < r - l);
  int ret= 0;
  for (auto h= lg; h--;)
   if (auto l0= mat[h].rank0(l), r0= mat[h].rank0(r); k >= r0 - l0) k-= r0 - l0, ret|= 1 << h, l+= mat[h].zeros - l0, r+= mat[h].zeros - r0;
   else l= l0, r= r0;
  return vec[ret];
 }
 // k-th(0-indexed) largest number in v[l,r)
 T kth_largest(int l, int r, int k) const { return kth_smallest(l, r, r - l - k - 1); }
 // count i s.t. (l <= i < r) && (v[i] < ub)
 int count(int l, int r, T ub) const {
  int x= std::lower_bound(vec.begin(), vec.end(), ub) - vec.begin();
  if (x >= 1u << lg) return r - l;
  if (x == 0) return 0;
  int ret= 0;
  for (auto h= lg; h--;)
   if (auto l0= mat[h].rank0(l), r0= mat[h].rank0(r); (x >> h) & 1) ret+= r0 - l0, l+= mat[h].zeros - l0, r+= mat[h].zeros - r0;
   else l= l0, r= r0;
  return ret;
 }
 // count i s.t. (l <= i < r) && (lb <= v[i] < ub)
 int count(int l, int r, T lb, T ub) const { return count(l, r, ub) - count(l, r, lb); }
};