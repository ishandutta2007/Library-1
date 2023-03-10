#pragma once
#include <iostream>
#include <set>
#include <iterator>
#include <cassert>
template <class Int, bool merge= true> class RangeSet {
 struct ClosedSection {
  Int l, r;
  Int length() const { return r - l + 1; }
  bool operator<(const ClosedSection &cs) const { return l < cs.l || (l == cs.l && r > cs.r); }
  operator bool() const { return l <= r; }
  friend std::ostream &operator<<(std::ostream &os, const ClosedSection &cs) { return cs ? os << "[" << cs.l << "," << cs.r << "]" : os << "∅"; }
 };
 std::set<ClosedSection> mp;
public:
 RangeSet() {
  constexpr Int INF= std::numeric_limits<Int>::max() / 2;
  mp.insert({INF, INF}), mp.insert({-INF, -INF});
 }
 ClosedSection covered_by(Int l, Int r) const {
  assert(l <= r);
  if (auto it= std::prev(mp.upper_bound(ClosedSection{l, l})); it->l <= l && r <= it->r) return *it;
  return {1, 0};
 }
 ClosedSection covered_by(Int x) const { return covered_by(x, x); }
 ClosedSection covered_by(const ClosedSection &cs) const { return covered_by(cs.l, cs.r); }
 size_t size() const { return mp.size() - 2; }
 auto begin() const { return std::next(mp.begin()); }
 auto end() const { return std::prev(mp.end()); }
 Int insert(Int l, Int r) {
  assert(l <= r);
  auto it= std::prev(mp.upper_bound(ClosedSection{l, l}));
  Int sum= 0, x= it->l, y= it->r;
  if (x <= l && r <= y) return sum;
  if (x <= l && l <= y + merge) sum+= y - (l= x) + 1, it= mp.erase(it);
  else std::advance(it, 1);
  for (; it->r < r; it= mp.erase(it)) sum+= it->r - it->l + 1;
  if (x= it->l, y= it->r; x - merge <= r && r <= y) sum+= (r= y) - x + 1, mp.erase(it);
  return mp.insert({l, r}), r - l + 1 - sum;
 }
 Int insert(Int x) { return insert(x, x); }
 Int insert(const ClosedSection &cs) { return insert(cs.l, cs.r); }
 Int erase(Int l, Int r) {
  assert(l <= r);
  auto it= std::prev(mp.upper_bound(ClosedSection{l, l}));
  Int sum= 0, x= it->l, y= it->r;
  if (x <= l && r <= y) {
   if (mp.erase(it); x < l) mp.insert({x, l - 1});
   if (r < y) mp.insert({r + 1, y});
   return r - l + 1;
  }
  if (x <= l && l <= y) {
   if (x < l) mp.insert({x, l - 1});
   sum+= y - l + 1, it= mp.erase(it);
  } else std::advance(it, 1);
  for (; it->r <= r; it= mp.erase(it)) sum+= it->r - it->l + 1;
  if (x= it->l, y= it->r; x <= r && r <= y)
   if (sum+= r - x + 1, mp.erase(it); r < y) mp.insert({r + 1, y});
  return sum;
 }
 Int erase(Int x) { return erase(x, x); }
 Int erase(const ClosedSection &cs) { return erase(cs.l, cs.r); }
 Int mex(Int x) const {
  auto cs= covered_by(x);
  return cs ? cs.r + 1 : x;
 }
 friend std::ostream &operator<<(std::ostream &os, const RangeSet &rs) {
  os << "[";
  for (auto it= rs.begin(); it != rs.end(); ++it) os << (it == rs.begin() ? "" : ",") << *it;
  return os << "]";
 }
};