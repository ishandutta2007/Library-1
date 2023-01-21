#pragma once
#include <vector>
#include <algorithm>
#include <numeric>
template <typename S> struct AhoCorasick {
 using symbol_t= S;
 template <class Array> AhoCorasick(const std::vector<Array> &ps) {
  static_assert(std::is_convertible_v<decltype(ps[0][0]), symbol_t>);
  const size_t n= ps.size();
  std::vector<int> ord(n), rows;
  std::iota(ord.begin(), ord.end(), 0), std::sort(ord.begin(), ord.end(), [&](int l, int r) { return ps[l] < ps[r]; });
  std::vector<size_t> lcp(n, 0), prev(n, 0), cur(n);
  for (size_t i= 1, j, ed; i < n; lcp[i++]= j)
   for (j= 0, ed= std::min(ps[ord[i - 1]].size(), ps[ord[i]].size()); j < ed; j++)
    if (ps[ord[i - 1]][j] != ps[ord[i]][j]) break;
  size_t nodes= 1;
  for (size_t i= 0; i < n; i++) nodes+= ps[ord[i]].size() - lcp[i];
  beg.reserve(nodes + 1), es.reserve(nodes), match.reserve(nodes), rows.reserve(n + 1);
  for (size_t row= 0; row < n; row++)
   if (!ps[ord[row]].empty()) rows.push_back(row);
  rows.push_back(-1), beg.push_back(0), match.push_back({});
  for (int i= 0; i < n && ps[ord[i]].empty(); i++) match[0].push_back(ord[i]);
  for (size_t col= 0; rows[0] != -1; col++) {
   int size= 0;
   for (int &r: rows) {
    if (r == -1) break;
    size_t row= r;
    if (size++; lcp[row] <= col) {
     if (size_t par= prev[row]; beg[par] == -1) beg[par]= es.size();
     es.push_back(ps[ord[row]][col]), beg.push_back(-1);
     if (match.push_back({}); col + 1 == ps[ord[row]].size())
      for (int i= row; i < n && ps[ord[i]] == ps[ord[row]]; i++) match.back().push_back(ord[i]);
    }
    if (cur[row]= beg.size() - 1; col + 1 == ps[ord[row]].size()) r= -1;
   }
   *std::remove(rows.begin(), rows.begin() + size, -1)= -1, prev.swap(cur);
  }
  beg.push_back(es.size());
  for (size_t i= beg.size() - 1; --i;)
   if (beg[i] == -1) beg[i]= beg[i + 1];
  fail.assign(match.size(), -1);
  for (int u= 0, ed= match.size(); u < ed; u++)
   for (auto i= beg[u], v= i + 1; i < beg[u + 1]; i++, v++) {
    int r= fail[v]= transition(fail[u], es[i]);
    match[v].insert(match[v].end(), match[r].begin(), match[r].end());
   }
 }
 inline int initial_state() const { return 0; }
 inline std::vector<int> match_patterns(int s) const { return match[s]; }
 inline bool is_accept(int s) const { return !match[s].empty(); }
 inline int transition(int u, symbol_t c) const {
  for (; u >= 0; u= fail[u])
   if (int v= next(u, c); v != -1) return v;
  return 0;
 }
 inline int state_size() const { return match.size(); }
private:
 std::vector<int> beg, fail;
 std::vector<symbol_t> es;
 std::vector<std::vector<int>> match;
 inline int next(int s, symbol_t c) const {
  int index= std::lower_bound(es.begin() + beg[s], es.begin() + beg[s + 1], c) - es.begin();
  if (index != beg[s + 1] && c == es[index]) return index + 1;
  return -1;
 }
};