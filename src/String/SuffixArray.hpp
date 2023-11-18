#pragma once
#include <string>
#include <algorithm>
#include <cassert>
#include "src/DataStructure/SparseTable.hpp"
template <bool use_lcp_query= false, class Int= int> struct SuffixArray {
 std::vector<int> SA, ISA, LCP;
 SuffixArray(const std::vector<Int> &S): s(S) {
  auto v= s;
  sort(v.begin(), v.end()), v.erase(unique(v.begin(), v.end()), v.end());
  std::vector<int> s_cpy(s.size() + 1);
  for (int i= s.size(); i--;) s_cpy[i]= std::lower_bound(v.begin(), v.end(), s[i]) - v.begin() + 1;
  SA= sa_is(s_cpy, v.size() + 1), SA.erase(SA.begin()), build();
 }
 SuffixArray(const std::string &S): s(S.begin(), S.end()) {
  std::vector<int> s_cpy(s.size() + 1);
  std::copy(s.begin(), s.end(), s_cpy.begin()), SA= sa_is(s_cpy, 128), SA.erase(SA.begin()), build();
 }
 // lcp of S[i:] and S[j:]
 int lcp(int i, int j) const {
  static_assert(use_lcp_query);
  assert(i < SA.size() && j < SA.size());
  if (i == j) return SA.size() - i;
  auto [l, r]= std::minmax(ISA[i], ISA[j]);
  return lcp_mn.fold(l, r);
 }
 int operator[](int i) const { return SA[i]; }
 int size() const { return SA.size(); }
 // return {l,r} s.t. P is a prefix of S[SA[i]:] ( i in [l,r) )
 // l == r if P is not a substring of S
 // O(|P|log|S|)
 std::pair<int, int> pattern_matching(const std::vector<Int> &P) const {
  const int n= s.size(), m= P.size();
  if (n < m) return {0, 0};
  auto f1= [&](int h) {
   for (int j= 0; h + j < n && j < m; ++j) {
    if (s[h + j] < P[j]) return true;
    if (s[h + j] > P[j]) return false;
   }
   return n - h < m;
  };
  auto f2= [&](int h) {
   for (int j= 0; h + j < n && j < m; ++j)
    if (s[h + j] > P[j]) return false;
   return true;
  };
  auto L= std::partition_point(SA.begin(), SA.end(), f1), R= std::partition_point(L, SA.end(), f2);
  return {L - SA.begin(), R - SA.begin()};
 }
 std::pair<int, int> pattern_matching(const std::string &P) const { return pattern_matching(std::vector<Int>(P.begin(), P.end())); }
private:
 std::vector<Int> s;
 using ST= SparseTable<int, int (*)(int, int)>;
 ST lcp_mn;
 void build() {
  const int n= SA.size();
  ISA.resize(n), LCP.resize(n - 1);
  for (int i= SA.size(); i--;) ISA[SA[i]]= i;
  for (int i= 0, h= 0; i < n; ++i) {
   if (ISA[i] == n - 1) {
    h= 0;
    continue;
   }
   for (int j= SA[ISA[i] + 1]; i + h < n && j + h < n && s[i + h] == s[j + h];) ++h;
   if ((LCP[ISA[i]]= h)) --h;
  }
  if constexpr (use_lcp_query) lcp_mn= ST(LCP, [](int a, int b) { return std::min(a, b); });
 }
 std::vector<int> static sa_is(const std::vector<int> &s, int K) {
  const int n= s.size();
  std::vector<char> t(n);
  std::vector<int> bkt(K, 0), bkt_l(K), bkt_r(K), sa(n), p1;
  t.back()= true;
  for (int i= n; --i;)
   if (t[i - 1]= (s[i - 1] < s[i] || (t[i] && s[i - 1] == s[i])); t[i] && !t[i - 1]) p1.push_back(i);
  std::reverse(p1.begin(), p1.end());
  const int n1= p1.size();
  for (int i= n; i--;) ++bkt[s[i]];
  for (int i= 0, sum= 0; i < K; ++i) sum+= bkt[i], bkt_r[i]= sum, bkt_l[i]= sum - bkt[i];
  std::vector<int> s1(n1), sa1(n1);
  std::fill_n(sa.begin(), n, -1), std::copy_n(bkt_r.begin(), K, bkt.begin());
  for (int i= n1; i--;) sa[--bkt[s[p1[i]]]]= p1[i];
  std::copy_n(bkt_l.begin(), K, bkt.begin());
  for (int i= 0, j; i < n; ++i)
   if ((j= sa[i] - 1) >= 0 && !t[j]) sa[bkt[s[j]]++]= j;
  std::copy_n(bkt_r.begin(), K, bkt.begin());
  for (int i= n, j; i--;)
   if ((j= sa[i] - 1) >= 0 && t[j]) sa[--bkt[s[j]]]= j;
  for (int i= 0, j= 0; i < n; ++i)
   if (t[sa[i]] && sa[i] > 0 && !t[sa[i] - 1]) sa1[j++]= sa[i];
  int name= 0;
  for (int i= 0, prev= -1, j, pos; i < n1; ++i, sa[pos]= name - 1)
   for (j= 0, pos= sa1[i];; ++j)
    if (prev == -1 || s[pos + j] != s[prev + j] || t[pos + j] != t[prev + j]) {
     prev= pos, ++name;
     break;
    } else if (j && ((t[pos + j] && !t[pos + j - 1]) || (t[prev + j] && !t[prev + j - 1]))) break;
  for (int i= n1; i--;) s1[i]= sa[p1[i]];
  if (name != n1) sa1= sa_is(s1, name);
  else
   for (int i= n1; i--;) sa1[s1[i]]= i;
  std::copy_n(bkt_r.begin(), K, bkt.begin()), std::fill_n(sa.begin(), n, -1);
  for (int i= n1; i--;) sa[--bkt[s[p1[sa1[i]]]]]= p1[sa1[i]];
  for (int i= 0, j; i < n; ++i)
   if ((j= sa[i] - 1) >= 0 && !t[j]) sa[bkt_l[s[j]]++]= j;
  for (int i= n, j; i--;)
   if ((j= sa[i] - 1) >= 0 && t[j]) sa[--bkt_r[s[j]]]= j;
  return sa;
 }
};