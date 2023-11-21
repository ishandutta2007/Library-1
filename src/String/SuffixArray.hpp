#pragma once
#include <string>
#include <vector>
#include <algorithm>
template <class String> struct SuffixArray {
 String s;
 std::vector<int> sa;
 static inline std::vector<int> sa_is(const std::vector<int> &s, int K) {
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
public:
 SuffixArray(const String &S): s(S) {
  std::vector<int> s_cpy(s.size() + 1);
  if constexpr (std::is_convertible_v<String, std::string>) std::copy(s.begin(), s.end(), s_cpy.begin()), sa= sa_is(s_cpy, 128), sa.erase(sa.begin());
  else {
   auto v= s;
   sort(v.begin(), v.end()), v.erase(unique(v.begin(), v.end()), v.end());
   for (int i= s.size(); i--;) s_cpy[i]= std::lower_bound(v.begin(), v.end(), s[i]) - v.begin() + 1;
   sa= sa_is(s_cpy, v.size() + 1), sa.erase(sa.begin());
  }
 }
 int operator[](int i) const { return sa[i]; }
 size_t size() const { return sa.size(); }
 auto begin() const { return sa.begin(); }
 auto end() const { return sa.end(); }
 // return {l,r} s.t. P is a prefix of S[sa[i]:] ( i in [l,r) )
 // l == r if P is not a substring of S
 // O(|P|log|S|)
 std::pair<int, int> pattern_matching(const String &P) const {
  const int n= s.size(), m= P.size();
  if (n < m) return {0, 0};
  auto f1= [&](int h) {
   auto t= s.begin() + h;
   for (int j= 0, e= std::min(n - h, m); j < e; ++j) {
    if (t[j] < P[j]) return true;
    if (t[j] > P[j]) return false;
   }
   return n - h < m;
  };
  auto f2= [&](int h) {
   auto t= s.begin() + h;
   for (int j= 0, e= std::min(n - h, m); j < e; ++j)
    if (t[j] > P[j]) return false;
   return true;
  };
  auto L= std::partition_point(sa.begin(), sa.end(), f1), R= std::partition_point(L, sa.end(), f2);
  return {L - sa.begin(), R - sa.begin()};
 }
};
class LCPArray {
 std::vector<int> rnk;
 std::vector<std::vector<int>> dat;
public:
 template <class String> LCPArray(const SuffixArray<String> &sa): rnk(sa.size()) {
  const int n= sa.size(), log= n > 2 ? 31 - __builtin_clz(n - 2) : 0;
  dat.resize(log + 1), dat[0].resize(n - 1);
  auto &lcp= dat[0];
  for (int i= n; i--;) rnk[sa[i]]= i;
  for (int i= 0, h= 0; i < n; ++i) {
   if (rnk[i] == n - 1) {
    h= 0;
    continue;
   }
   for (int j= sa[rnk[i] + 1]; i + h < n && j + h < n && sa.s[i + h] == sa.s[j + h];) ++h;
   if ((lcp[rnk[i]]= h)) --h;
  }
  for (int i= 0, I= 1, j; i < log; ++i, I<<= 1)
   for (dat[i + 1].resize(j= dat[i].size() - I); j--;) dat[i + 1][j]= std::min(dat[i][j], dat[i][j + I]);
 }
 int operator[](int i) const { return dat[0][i]; }
 size_t size() const { return dat[0].size(); }
 auto begin() const { return dat[0].begin(); }
 auto end() const { return dat[0].end(); }
 int operator()(int i, int j) const {
  if (i == j) return rnk.size() - i;
  auto [l, r]= std::minmax(rnk[i], rnk[j]);
  if (r == l + 1) return dat[0][l];
  int k= 31 - __builtin_clz(r - l - 1);
  return std::min(dat[k][l], dat[k][r - (1 << k)]);
 }
};