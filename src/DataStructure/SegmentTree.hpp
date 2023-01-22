#pragma once
#include <vector>
#include <cassert>
template <typename M> struct SegmentTree {
 using T= typename M::T;
 SegmentTree() {}
 SegmentTree(int n_): n(n_), dat(n << 1, M::ti()) {}
 SegmentTree(int n_, T v): n(n_), dat(n << 1, M::ti()) {
  for (int i= n; i--;) dat[i + n]= v;
  rebuild();
 }
 SegmentTree(const std::vector<T> &v): n(v.size()), dat(n << 1, M::ti()) {
  for (int i= n; i--;) dat[i + n]= v[i];
  rebuild();
 }
 void set(int k, T x) {
  for (dat[k+= n]= x; k>>= 1;) dat[k]= M::op(dat[(k << 1) | 0], dat[(k << 1) | 1]);
 }
 void unsafe_set(int k, T x) { dat[k + n]= x; }
 void rebuild() {
  for (int i= n; --i;) dat[i]= M::op(dat[i << 1 | 0], dat[i << 1 | 1]);
 }
 void clear() { fill(dat.begin(), dat.end(), M::ti()); }
 inline T fold(int l, int r) const {  //[l,r)
  T vl= M::ti(), vr= M::ti();
  for (int a= l + n, b= r + n; a < b; a>>= 1, b>>= 1) {
   if (a & 1) vl= M::op(vl, dat[a++]);
   if (b & 1) vr= M::op(dat[--b], vr);
  }
  return M::op(vl, vr);
 }
 T operator[](const int &k) const { return dat[k + n]; }
 template <bool last> static inline T calc_op(const T &v, const T &d) {
  if constexpr (last) return M::op(d, v);
  else return M::op(v, d);
 }
 // Case 0. find i s.t check(fold(k,i)) == False, check(fold(k,i+1)) == True
 // Case 1. find i s.t check(fold(i+1,b)) == False, check(fold(i,b)) == True
 // return -1 if not found
 template <bool last, class C> int find(const C &check, int k) const {
  assert(!check(M::ti()));
  std::vector<int> id[2];
  int a= n + (k & -(!last)), b= n + n + ((k - n) & -(last));
  for (; a < b; a>>= 1, b>>= 1) {
   if (a & 1) id[0].push_back(a++);
   if (b & 1) id[1].push_back(--b);
  }
  id[last].insert(id[last].end(), id[!last].rbegin(), id[!last].rend());
  T val= M::ti();
  for (int i: id[last]) {
   if (T tmp= calc_op<last>(val, dat[i]); check(tmp)) {
    while (i < n)
     if (tmp= calc_op<last>(val, dat[i= i << 1 | last]); !check(tmp)) val= tmp, i-= last * 2 - 1;
    return i - n + last;
   } else val= tmp;
  }
  return -1;
 }
private:
 const int n;
 std::vector<T> dat;
};