#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include <tuple>
template <class T> std::vector<int> z_algorithm(const std::vector<T> &s) {
 const int n= s.size();
 if (n == 0) return {};
 std::vector<int> z(n);
 for (int i= 1, j= 0; i < n; ++i) {
  int &k= z[i];
  for (k= (j + z[j] <= i) ? 0 : std::min(j + z[j] - i, z[i - j]); i + k < n && s[k] == s[i + k];) ++k;
  if (j + z[j] < i + z[i]) j= i;
 }
 return z[0]= n, z;
}
std::vector<int> z_algorithm(const std::string &s) { return z_algorithm(std::vector<int>(s.begin(), s.end())); }
namespace string_internal {
template <class String> auto run_enumerate_(const String &s, typename String::value_type a= 0) {
 using Run= std::tuple<int, int, int>;
 std::vector<Run> glb;
 auto rec= [&](auto rec, int l, int r) -> std::vector<Run> {
  if (r - l <= 1) return {};
  const int m= (l + r) / 2, lsz= m - l, rsz= r - m;
  std::vector<Run> run_l= lsz > 1 ? rec(rec, l, m) : std::vector<Run>(), run_r= rsz > 1 ? rec(rec, m, r) : std::vector<Run>();
  std::vector<typename String::value_type> rl(r - l + 1, a);
  std::copy(s.begin() + m, s.begin() + r, rl.begin()), std::copy(s.begin() + l, s.begin() + m, rl.begin() + rsz + 1);
  std::vector<int> zrl= z_algorithm(rl);
  std::reverse(rl.begin(), rl.end());
  std::vector<int> zrl_r= z_algorithm(rl);
  const int sz= rl.size();
  std::vector<Run> ret;
  auto push= [&](int p, int b, int e) { (b == l || e == r ? ret : glb).emplace_back(p, b, e); };
  std::vector<Run> run_m(rsz / 2 + 1);
  for (auto [p, b, e]: run_r) {
   if (b != m) ret.emplace_back(p, b, e);
   else run_m[p]= Run{p, b, e};
  }
  for (auto [p, b, e]: run_l)
   if (e != m) ret.emplace_back(p, b, e);
   else if (zrl[sz - p] == p) {
    if (std::get<0>(run_m[p])) push(p, b, std::get<2>(run_m[p])), run_m[p]= Run{};
    else push(p, b, m + p + zrl[p]);
   } else push(p, b, m + zrl[sz - p]);
  for (auto [p, b, e]: run_m)
   if (p) {
    if (zrl[sz - p] != p) push(p, m - zrl_r[sz - p], e);
    else if (2 * p > lsz || zrl[sz - 2 * p] < p) push(p, m - p - zrl_r[p], e);
   }
  for (int p= 1; p <= lsz; ++p)
   if (bool skpr= 2 * p <= rsz && zrl[p] >= p, skpl= 2 * p <= lsz && zrl[sz - 2 * p] >= p; zrl[sz - p] == p) {
    if (!skpl && !skpr) push(p, m - p - zrl_r[p], m + p + zrl[p]);
   } else {
    if (!skpr)
     if (int b= m - zrl_r[sz - p], e= m + p + zrl[p]; e - b >= 2 * p) push(p, b, e);
    if (!skpl)
     if (int b= m - p - zrl_r[p], e= m + zrl[sz - p]; e - b >= 2 * p) push(p, b, e);
   }
  return ret;
 };
 std::vector<std::tuple<int, int, int>> runs= rec(rec, 0, s.size());
 std::copy(glb.begin(), glb.end(), std::back_inserter(runs));
 int rn= runs.size(), n= s.size();
 glb.resize(rn);
 std::vector<int> pt(n);
#define RADIX(A, B, C, D, E) \
 for (auto [p, l, r]: A) ++pt[C]; \
 for (int i= 1; E < n; ++i) pt[i]+= pt[i - 1]; \
 for (int i= rn; i--;) B[--pt[D]]= A[i];
 RADIX(runs, glb, r - 1, std::get<2>(runs[i]) - 1, i)
 pt.assign(n, 0);
 RADIX(glb, runs, l, std::get<1>(glb[i]), i)
 runs.erase(std::unique(runs.begin(), runs.end(), [](auto &r1, auto &r2) { return std::get<1>(r1) == std::get<1>(r2) && std::get<2>(r1) == std::get<2>(r2); }), runs.end()), glb.resize(rn= runs.size()), pt.assign(n / 2, 0);
 RADIX(runs, glb, p - 1, std::get<0>(runs[i]) - 1, i + i)
#undef RADIX
 return glb;
}
template <class Int> auto run_enumerate(const std::vector<Int> &s) { return run_enumerate_(s, *std::max_element(s.begin(), s.end()) + 1); }
auto run_enumerate(const std::string &s) { return run_enumerate_(s); }
}
using string_internal::run_enumerate;