#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include <tuple>
template <class String> std::vector<int> z_algorithm(const String &s) {
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
std::vector<int> z_to_kmp(const std::vector<int> &z) {
 const int n= z.size();
 std::vector<int> kmp(n + 1);
 kmp[0]= -1;
 for (int i= n; --i;) kmp[i + z[i]]= std::max(kmp[i + z[i]], z[i]);
 for (int i= n; i--;) kmp[i]= std::max(kmp[i], kmp[i + 1] - 1);
 return kmp;
}
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
  auto push= [&](int b, int e, int p) { (b == l || e == r ? ret : glb).emplace_back(b, e, p); };
  std::vector<Run> run_m(rsz / 2 + 1);
  for (auto [b, e, p]: run_r) {
   if (b != m) ret.emplace_back(b, e, p);
   else run_m[p]= Run{b, e, p};
  }
  for (auto [b, e, p]: run_l)
   if (e != m) ret.emplace_back(b, e, p);
   else if (zrl[sz - p] == p) {
    if (std::get<2>(run_m[p])) push(b, std::get<1>(run_m[p]), p), run_m[p]= Run{};
    else push(b, m + p + zrl[p], p);
   } else push(b, m + zrl[sz - p], p);
  for (auto [b, e, p]: run_m)
   if (p) {
    if (zrl[sz - p] != p) push(m - zrl_r[sz - p], e, p);
    else if (2 * p > lsz || zrl[sz - 2 * p] < p) push(m - p - zrl_r[p], e, p);
   }
  for (int p= 1; p <= lsz; ++p)
   if (bool skpr= 2 * p <= rsz && zrl[p] >= p, skpl= 2 * p <= lsz && zrl[sz - 2 * p] >= p; zrl[sz - p] == p) {
    if (!skpl && !skpr) push(m - p - zrl_r[p], m + p + zrl[p], p);
   } else {
    if (!skpr)
     if (int b= m - zrl_r[sz - p], e= m + p + zrl[p]; e - b >= 2 * p) push(b, e, p);
    if (!skpl)
     if (int b= m - p - zrl_r[p], e= m + zrl[sz - p]; e - b >= 2 * p) push(b, e, p);
   }
  return ret;
 };
 std::vector<std::tuple<int, int, int>> runs= rec(rec, 0, s.size());
 std::copy(glb.begin(), glb.end(), std::back_inserter(runs)), std::sort(runs.begin(), runs.end()), runs.erase(std::unique(runs.begin(), runs.end(), [](auto &r1, auto &r2) { return std::get<0>(r1) == std::get<0>(r2) && std::get<1>(r1) == std::get<1>(r2); }), runs.end());
 int rn= runs.size(), n= s.size();
 glb.resize(rn);
 std::vector<int> pt(n);
 for (auto [l, r, p]: runs) ++pt[p];
 for (int i= 1; i < n; ++i) pt[i]+= pt[i - 1];
 for (int i= rn; i--;) glb[--pt[std::get<2>(runs[i])]]= {std::get<2>(runs[i]), std::get<0>(runs[i]), std::get<1>(runs[i])};
 return glb;
}
template <class Int, typename= std::enable_if_t<std::is_integral_v<Int>>> auto run_enumerate(const std::vector<Int> &s) { return run_enumerate_(s, *std::max_element(s.begin(), s.end()) + 1); }
template <class String> auto run_enumerate(const String &s) {
 auto v= s;
 std::sort(v.begin(), v.end()), v.erase(std::unique(v.begin(), v.end()), v.end());
 std::vector<int> t(s.size());
 for (int i= s.size(); i--;) t[i]= std::lower_bound(v.begin(), v.end(), s[i]) - v.begin();
 return run_enumerate_(t, v.size() + 1);
}
auto run_enumerate(const std::string &s) { return run_enumerate_(s); }
}
using string_internal::run_enumerate;