#pragma once
#include <bits/stdc++.h>
template <class K> class RollingHash {
 static inline std::vector<K> pw;
 static inline K base;
 static inline void set_pw(int n) {
  if (int m= pw.size(); m < n)
   for (pw.resize(n); m < n; m++) pw[m]= pw[m - 1] * base;
 }
 std::vector<K> hash;
public:
 class SubString {
  const RollingHash *instance;
  const int bg, ed;
 public:
  SubString(const RollingHash &rh): instance(&rh), bg(0), ed(rh.hash.size()) {}
  SubString(const RollingHash *i, int b, int e): instance(i), bg(b), ed(e) {}
  inline K get_hash(int l= 0, int r= -1) const { return instance->get_hash(bg + l, (r == -1 ? ed : bg + r)); }
  friend int lcp(const SubString &l, const SubString &r) {
   int ok= 0, ng= std::min(l.ed - l.bg, r.ed - r.bg) + 1;
   for (int x; ng - ok > 1;) x= (ok + ng) / 2, (l.get_hash(0, x) == r.get_hash(0, x) ? ok : ng)= x;
   return ok;
  }
 };
 static void set_base(K b) { base= b, pw.assign(1, 1); }
 static K base_pow(int i) { return pw[i]; }
 RollingHash()= default;
 template <class T> RollingHash(const std::vector<T> &v): hash(v.size() + 1, 0) {
  set_pw(hash.size());
  for (int i= 0, ed= v.size(); i < ed; i++) hash[i + 1]= hash[i] * base + v[i];
 }
 RollingHash(const std::string &s): RollingHash(std::vector<char>(s.begin(), s.end())) {}
 inline K get_hash(int l= 0, int r= -1) const {
  if (r < 0) r= hash.size() - 1;
  return hash[r] - hash[l] * pw[r - l];
 }
 SubString sub(int l, int r) const { return SubString{this, l, r}; }
};
std::uint64_t get_rand(std::uint64_t l, std::uint64_t r) {
 static std::mt19937_64 gen(std::random_device{}());
 return std::uniform_int_distribution<std::uint64_t>(l, r)(gen);
}