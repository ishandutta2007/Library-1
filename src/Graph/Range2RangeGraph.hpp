#pragma once
#include <vector>
#include <tuple>
template <typename T= int> class Range2RangeGraph {
 const int n;
 int nn;
 std::vector<std::tuple<int, int, T>> es;
 inline int to_upper_idx(int i) const { return i >= n ? i - n : n + i; }
 inline int to_lower_idx(int i) const { return i >= n ? i - n : n + n + i; }
public:
 Range2RangeGraph(int n): n(n), nn(n * 3) {
  for (int i= 2; i < n + n; ++i) add(to_upper_idx(i / 2), to_upper_idx(i));
  for (int i= 2; i < n + n; ++i) add(to_lower_idx(i), to_lower_idx(i / 2));
 }
 void add(int s, int t, T w= 0) { es.emplace_back(s, t, w); }
 // [s_l, s_r) -> t
 void add_from_range(int s_l, int s_r, int t, T w= 0) {
  for (int l= s_l + n, r= s_r + n; l < r; l>>= 1, r>>= 1) {
   if (l & 1) add(to_lower_idx(l++), t, w);
   if (r & 1) add(to_lower_idx(--r), t, w);
  }
 }
 // s -> [t_l, t_r)
 void add_to_range(int s, int t_l, int t_r, T w= 0) {
  for (int l= t_l + n, r= t_r + n; l < r; l>>= 1, r>>= 1) {
   if (l & 1) add(s, to_upper_idx(l++), w);
   if (r & 1) add(s, to_upper_idx(--r), w);
  }
 }
 // [s_l, s_r) -> [t_l, t_r)
 void add_from_range_to_range(int s_l, int s_r, int t_l, int t_r, T w= 0) { add_from_range(s_l, s_r, nn, w), add_to_range(nn, t_l, t_r, 0), ++nn; }
 int node_size() const { return nn; }
 std::vector<std::tuple<int, int, T>> get_edges() const { return es; }
};