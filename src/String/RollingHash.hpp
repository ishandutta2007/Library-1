#pragma once
#include <vector>
#include <string>
#include <array>
#include <cassert>
template <class K, class Int= int> class RollingHash {
 static inline std::vector<K> pw, hsh;
 static inline K bs;
 static inline std::vector<Int> str;
 static inline void set_pw(int n) {
  if (int m= pw.size(); m <= n)
   for (pw.resize(n + 1); m <= n; ++m) pw[m]= pw[m - 1] * bs;
 }
 int bg, n;
 RollingHash(int b, int n): bg(b), n(n) {}
 template <class C> static int bin_srch(int ok, int ng, const C &check) {
  for (int x; ng - ok > 1;) x= (ok + ng) / 2, (check(x) ? ok : ng)= x;
  return ok;
 }
 template <size_t I> static K concat(const std::array<RollingHash, I> &v) {
  K ret= 0;
  for (int i= 0; i < I; ++i) ret= ret * pw[v[i].n] + v[i].hash();
  return ret;
 }
public:
 static void init(K b) { bs= b, pw.assign(1, 1), hsh.assign(1, 0); }
 static K base_pow(int i) { return set_pw(i), pw[i]; }
 RollingHash()= default;
 RollingHash(const std::vector<Int> &v): bg(hsh.size() - 1), n(v.size()) {
  str.insert(str.end(), v.begin(), v.end()), set_pw(n), hsh.resize(bg + n + 1);
  for (int i= 0; i < n; ++i) hsh[bg + i + 1]= hsh[bg + i] * bs + v[i];
 }
 RollingHash(const std::string &s): RollingHash(std::vector<Int>(s.begin(), s.end())) {}
 inline size_t length() const { return n; }
 inline K hash() const { return hsh[bg + n] - hsh[bg] * pw[n]; }
 RollingHash sub(int b, int m) const {
  assert(b + m <= n), assert(m >= 0);
  return {bg + b, m};
 }
 RollingHash sub(int b) const {
  assert(b <= n);
  return {bg + b, n - b};
 }
 template <class... Args> friend std::enable_if_t<std::conjunction_v<std::is_same<Args, RollingHash>...>, K> concat_hash(const Args &...rh) { return concat(std::array{rh...}); }
 friend int lcp(const RollingHash &l, const RollingHash &r) {
  return bin_srch(0, std::min(l.n, r.n) + 1, [&](int x) { return l.sub(0, x) == r.sub(0, x); });
 }
 friend int lcs(const RollingHash &l, const RollingHash &r) {
  return bin_srch(0, std::min(l.n, r.n) + 1, [&](int x) { return l.sub(l.n - x) == r.sub(r.n - x); });
 }
 bool operator==(const RollingHash &r) const { return hash() == r.hash(); }
 bool operator!=(const RollingHash &r) const { return !(*this == r); }
 bool operator<(const RollingHash &r) const {
  int k= lcp(*this, r);
  return k == std::min(n, r.n) ? n < r.n : str[bg + k] < str[r.bg + k];
 }
 friend bool concat_cmp(const RollingHash &l, const RollingHash &r) {
  int k= lcp(l, r);
  if (l.n < r.n) {
   if (k < l.n) return str[l.bg + k] < str[r.bg + k];
   if (k= lcp(r, r.sub(l.n)); k < r.n - l.n) return str[r.bg + k] < str[r.bg + l.n + k];
   if (k= lcp(r.sub(r.n - l.n), l); k < l.n) return str[r.bg + r.n - l.n + k] < str[l.bg + k];
  } else {
   if (k < r.n) return str[l.bg + k] < str[r.bg + k];
   if (k= lcp(l.sub(r.n), l); k < l.n - r.n) return str[l.bg + r.n + k] < str[l.bg + k];
   if (k= lcp(r, l.sub(l.n - r.n)); k < r.n) return str[r.bg + k] < str[l.bg + l.n - r.n + k];
  }
  return false;
 }
 std::string to_str() const {  // for debug
  std::string ret;
  for (int i= bg; i < bg + n; ++i) ret+= (char)str[i];
  return ret;
 }
};