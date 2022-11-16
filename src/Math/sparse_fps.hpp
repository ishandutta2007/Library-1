#pragma once
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/mod_sqrt.hpp"
/**
 * @title 疎な形式的冪級数
 * @category 数学
 * @see
 * https://maspypy.com/%E5%A4%9A%E9%A0%85%E5%BC%8F%E3%83%BB%E5%BD%A2%E5%BC%8F%E7%9A%84%E3%81%B9%E3%81%8D%E7%B4%9A%E6%95%B0-%E9%AB%98%E9%80%9F%E3%81%AB%E8%A8%88%E7%AE%97%E3%81%A7%E3%81%8D%E3%82%8B%E3%82%82%E3%81%AE
 */

// verify用:
// https://judge.yosupo.jp/problem/sqrt_of_formal_power_series_sparse

// BEGIN CUT HERE
template <class K>
std::vector<K> sparse_inv(const std::vector<K> &f, int n) {
  assert(f[0] != K(0));
  std::vector<std::pair<int, K>> dat;
  for (int i = 1, ed = std::min<int>(n, f.size()); i < ed; i++)
    if (f[i] != K(0)) dat.emplace_back(i, f[i]);
  std::vector<K> ret(n);
  const K iv = ret[0] = K(1) / f[0];
  for (int i = 1; i < n; ret[i++] *= iv)
    for (auto &&[j, v] : dat) {
      if (i < j) break;
      ret[i] -= ret[i - j] * v;
    }
  return ret;
}

template <class K>
std::vector<K> sparse_div(std::vector<K> f, const std::vector<K> &g, int n) {
  assert(g[0] != K(0));
  std::vector<std::pair<int, K>> dat;
  for (int i = 1, ed = std::min<int>(n, g.size()); i < ed; i++)
    if (g[i] != K(0)) dat.emplace_back(i, g[i]);
  f.resize(n);
  const K iv = K(1) / g[0];
  for (int i = 0; i < n; f[i++] *= iv)
    for (auto &&[j, v] : dat) {
      if (i < j) break;
      f[i] -= f[i - j] * v;
    }
  return f;
}

template <class mod_t, std::size_t _Nm = 1 << 24>
std::vector<mod_t> sparse_log(const std::vector<mod_t> &f, int n) {
  assert(f[0] == mod_t(1));
  std::vector<mod_t> df(n - 1);
  for (int i = 1, ed = std::min<int>(n, f.size()); i < ed; i++)
    df[i - 1] += f[i] * i;
  df = sparse_div(df, f, n - 1);
  std::vector<mod_t> ret(n);
  for (int i = 1; i < n; i++) ret[i] = df[i - 1] * get_inv<mod_t, _Nm>(i);
  return ret;
}

template <class mod_t, std::size_t _Nm = 1 << 24>
std::vector<mod_t> sparse_exp(const std::vector<mod_t> &f, int n) {
  assert(f[0] == mod_t(0));
  std::vector<std::pair<int, mod_t>> dat;
  for (int i = 1, ed = std::min<int>(n, f.size()); i < ed; i++)
    if (f[i] != mod_t(0)) dat.emplace_back(i - 1, f[i] * i);
  std::vector<mod_t> ret(n);
  ret[0] = 1;
  for (int i = 1; i < n; ret[i] *= get_inv<mod_t, _Nm>(i), i++)
    for (auto &&[j, v] : dat) {
      if (i <= j) break;
      ret[i] += ret[i - 1 - j] * v;
    }
  return ret;
}

template <class mod_t, std::size_t _Nm = 1 << 24>
std::vector<mod_t> sparse_pow(const std::vector<mod_t> &f, std::uint64_t k,
                              int n) {
  std::vector<mod_t> ret(n);
  if (k == 0) return ret[0] = 1, ret;
  int cnt = 0, ed = std::min<int>(n, f.size());
  while (cnt < ed && f[cnt] == mod_t(0)) cnt++;
  const __int128_t ofs = (__int128_t)k * cnt, sz = n - ofs;
  if (sz <= 0) return ret;
  std::vector<std::pair<int, mod_t>> dat;
  for (int i = cnt + 1; i < ed; i++)
    if (f[i] != mod_t(0)) dat.emplace_back(i - cnt, f[i]);
  mod_t *bf = ret.data() + k * cnt, mk = k, iv = mod_t(1) / f[cnt];
  bf[0] = f[cnt].pow(k);
  for (int i = 1; i < sz; bf[i] *= get_inv<mod_t, _Nm>(i) * iv, i++)
    for (auto &&[j, v] : dat) {
      if (i < j) break;
      bf[i] += v * (mk * j - (i - j)) * bf[i - j];
    }
  return ret;
}

template <class mod_t, std::size_t _Nm = 1 << 24>
std::vector<mod_t> sparse_sqrt(const std::vector<mod_t> &f, int n) {
  std::vector<mod_t> ret(n);
  int cnt = 0, ed = std::min<int>(n, f.size());
  while (cnt < ed && f[cnt] == mod_t(0)) cnt++;
  if (cnt == ed) return ret;
  if (cnt & 1) return {};  // no solution
  const int ofs = cnt >> 1, sz = n - ofs;
  std::vector<std::pair<int, mod_t>> dat;
  for (int i = cnt + 1; i < ed; i++)
    if (f[i] != mod_t(0)) dat.emplace_back(i - cnt, f[i]);
  mod_t *bf = ret.data() + ofs, mk = mod_t(1) / 2, iv = mod_t(1) / f[cnt];
  bf[0] = mod_sqrt(f[cnt].val(), mod_t::modulo());
  if (bf[0] * bf[0] != f[cnt]) return {};  // no solution
  for (int i = 1; i < sz; bf[i] *= get_inv<mod_t, _Nm>(i) * iv, i++)
    for (auto &&[j, v] : dat) {
      if (i < j) break;
      bf[i] += v * (mk * j - (i - j)) * bf[i - j];
    }
  return ret;
}

// F'/F = f(x)/g(x), F[0]=1
template <class mod_t, std::size_t _Nm = 1 << 24>
std::vector<mod_t> sparse_log_differentiation(const std::vector<mod_t> &f,
                                              const std::vector<mod_t> &g,
                                              int n) {
  assert(g[0] == mod_t(1));
  std::vector<std::pair<int, mod_t>> dat_f, dat_g;
  for (int i = 0, ed = std::min<int>(f.size(), n); i < ed; i++)
    if (f[i] != mod_t(0)) dat_f.emplace_back(i, f[i]);
  for (int i = 1, ed = std::min<int>(g.size(), n); i < ed; i++)
    if (g[i] != mod_t(0)) dat_g.emplace_back(i, g[i]);
  std::vector<mod_t> ret(n), d(n - 1);
  ret[0] = 1;
  for (int i = 0; i < n - 1; i++) {
    for (auto &&[j, v] : dat_g) {
      if (i < j) break;
      d[i] -= v * d[i - j];
    }
    for (auto &&[j, v] : dat_f) {
      if (i < j) break;
      d[i] += v * ret[i - j];
    }
    ret[i + 1] = d[i] * get_inv<mod_t, _Nm>(i + 1);
  }
  return ret;
}

template <class mod_t, std::size_t _Nm = 1 << 24>  // exp(f/g)
std::vector<mod_t> sparse_exp_of_div(const std::vector<mod_t> &f,
                                     const std::vector<mod_t> &g, int n) {
  assert(f[0] == mod_t(0)), assert(g[0] == mod_t(1));
  std::vector<std::pair<int, mod_t>> dat_f, dat_g;
  for (int i = 1, ed = std::min<int>(f.size(), n); i < ed; i++)
    if (f[i] != mod_t(0)) dat_f.emplace_back(i, f[i]);
  for (int i = 0, ed = std::min<int>(g.size(), n); i < ed; i++)
    if (g[i] != mod_t(0)) dat_g.emplace_back(i, g[i]);
  std::vector<mod_t> a(f.size() + g.size() - 2), b(2 * g.size() - 1);
  for (auto &&[i, x] : dat_f)
    for (auto &&[j, y] : dat_g) a[i + j - 1] += x * y * (i - j);
  for (auto &&[i, x] : dat_g)
    for (auto &&[j, y] : dat_g) b[i + j] += x * y;  // a = f'g-fg', b = g^2
  return sparse_log_differentiation<mod_t, _Nm>(a, b, n);
}

template <class mod_t, std::size_t _Nm = 1 << 24>  // (f/g)^k
std::vector<mod_t> sparse_pow_of_div(const std::vector<mod_t> &f,
                                     const std::vector<mod_t> &g,
                                     std::uint64_t k, int n) {
  assert(f[0] == mod_t(1)), assert(g[0] == mod_t(1));
  std::vector<std::pair<int, mod_t>> dat_f, dat_g;
  for (int i = 0, ed = std::min<int>(f.size(), n); i < ed; i++)
    if (f[i] != mod_t(0)) dat_f.emplace_back(i, f[i]);
  for (int i = 0, ed = std::min<int>(g.size(), n); i < ed; i++)
    if (g[i] != mod_t(0)) dat_g.emplace_back(i, g[i]);
  std::vector<mod_t> a(f.size() + g.size() - 2), b(2 * g.size() - 1);
  for (auto &&[i, x] : dat_f)
    for (auto &&[j, y] : dat_g) a[i + j - 1] += x * y * (i - j) * k;
  for (auto &&[i, x] : dat_f)
    for (auto &&[j, y] : dat_g) b[i + j] += x * y;  // a = k(f'g-fg'), b = fg
  return sparse_log_differentiation<mod_t, _Nm>(a, b, n);
}