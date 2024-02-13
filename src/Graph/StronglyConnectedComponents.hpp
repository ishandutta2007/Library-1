#pragma once
#include <algorithm>
#include <numeric>
#include <array>
#include "src/DataStructure/CsrArray.hpp"
class StronglyConnectedComponents {
 std::vector<std::array<int, 2>> es;
 std::vector<int> csr, pos, id;
public:
 StronglyConnectedComponents(int n): csr(n, -2), id(n) {}
 void add_edge(int src, int dst) { es.push_back({src, dst}); }
 void build() {
  const int n= id.size();
  std::vector<int> g(es.size()), sep(n + 1), ord(n);
  for (auto [s, d]: es) ++sep[s];
  for (int i= 0; i < n; ++i) sep[i + 1]+= sep[i];
  for (auto [s, d]: es) g[--sep[s]]= d;
  std::vector<int> dat(sep.begin(), sep.begin() + n);
  int k= n, p;
  for (int s= 0; s < n; ++s)
   if (csr[s] == -2)
    for (csr[p= s]= -1; p >= 0;) {
     if (dat[p] == sep[p + 1]) ord[--k]= p, p= csr[p];
     else if (int q= g[dat[p]++]; csr[q] == -2) csr[q]= p, p= q;
    }
  sep.assign(n + 1, 0), pos= {p= 0};
  for (auto [s, d]: es) ++sep[d];
  for (int i= 0; i < n; ++i) sep[i + 1]+= sep[i];
  for (auto [s, d]: es) g[--sep[d]]= s;
  for (int s: ord)
   if (dat[s] >= 0) {
    for (csr[k++]= s, dat[s]= -1; p < k; ++p)
     for (int v= csr[p], j= sep[v], u; j < sep[v + 1]; ++j)
      if (dat[u= g[j]] >= 0) dat[u]= -1, csr[k++]= u;
    pos.push_back(k);
   }
  for (int i= pos.size() - 1; i--;)
   while (k > pos[i]) id[csr[--k]]= i;
 }
 int components_num() const { return pos.size() - 1; }
 ConstListRange<int> block(int k) const { return {csr.cbegin() + pos[k], csr.cbegin() + pos[k + 1]}; }
 int belong(int i) const { return id[i]; }
 CsrArray<int> dag() const {
  std::vector<std::array<int, 2>> es_;
  for (auto [s, d]: es)
   if (s= id[s], d= id[d]; s != d) es_.push_back({s, d});
  std::sort(es_.begin(), es_.end()), es_.erase(std::unique(es_.begin(), es_.end()), es_.end());
  std::vector<int> g(es_.size()), p(pos.size());
  for (auto [s, d]: es_) ++p[s];
  std::partial_sum(p.begin(), p.end(), p.begin());
  for (auto [s, d]: es_) g[--p[s]]= d;
  return {g, p};
 }
};