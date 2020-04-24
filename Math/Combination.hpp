/**
 * @title 組み合わせ
 * @category 数学
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <class Modint>
struct Combination {
  static vector<Modint> _fact, _finv, _inv;
  static void init(int sz) {
    int n = min(sz, Modint::modulo() - 1);
    _fact.resize(n + 1), _finv.resize(n + 1), _inv.resize(n + 1);
    for (int i = 0; i <= n; ++i) _fact[i] = i ? Modint(i) * _fact[i - 1] : 1;
    _finv[n] = _fact[n].inverse();
    for (int i = n; i; --i) _finv[i - 1] = Modint(i) * _finv[i];
    for (int i = 1; i <= n; ++i) _inv[i] = _finv[i] * _fact[i - 1];
  }
  static Modint inv(int n) { return _inv[n]; }
  static Modint fact(int n) { return _fact[n]; }
  static Modint fact_inv(int n) { return _finv[n]; }
  static Modint C(int n, int k) {
    if (n < k || k < 0) return Modint(0);
    return _fact[n] * _finv[n - k] * _finv[k];
  }
  static Modint P(int n, int k) {
    if (n < k || k < 0) return Modint(0);
    return _fact[n] * _finv[n - k];
  }
  static Modint H(int n, int k) {
    if (n < 0 || k < 0) return Modint(0);
    if (!n && !k) return Modint(1);
    return C(n + k - 1, n);
  }
  static size_t size() { return _inv.size(); }
};
template <class Modint>
vector<Modint> Combination<Modint>::_fact;
template <class Modint>
vector<Modint> Combination<Modint>::_finv;
template <class Modint>
vector<Modint> Combination<Modint>::_inv;