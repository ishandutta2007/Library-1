#pragma once
#include <bits/stdc++.h>
#include "src/FFT/NTT.hpp"

/**
 * @title 形式的冪級数
 * @category FFT
 * @see https://hly1204.github.io/library/math/formal_power_series.hpp
 */

// BEGIN CUT HERE
template <class T, std::size_t _Nm = 1 << 22>
class RelaxedConvolution {
  std::vector<T> a, b, c;
  std::vector<NTTArray<T, _Nm, true>> ac, bc;
  std::function<T()> ha, hb;
  int n;
  template <class T0>
  static auto wrap(T0 &&f, int &n, const std::vector<T> &c, std::vector<T> &e) {
    if constexpr (std::is_invocable_r_v<T, T0, int, const std::vector<T> &>) {
      return std::bind(
          [f](int n, const std::vector<T> &c, std::vector<T> &e) mutable {
            return T(e.emplace_back(f(n, c)));
          },
          std::cref(n), std::cref(c), std::ref(e));
    } else if constexpr (std::is_invocable_r_v<T, T0, int>) {
      return std::bind(
          [f](int n, std::vector<T> &e) mutable {
            return T(e.emplace_back(f(n)));
          },
          std::cref(n), std::ref(e));
    } else if constexpr (std::is_invocable_r_v<T, T0>) {
      return std::bind(
          [f](std::vector<T> &e) mutable { return T(e.emplace_back(f())); },
          std::ref(e));
    } else
      throw;
  }

 public:
  template <class F1, class F2>
  RelaxedConvolution(F1 &&h1, F2 &&h2)
      : c(4), ha(wrap(h1, n, c, a)), hb(wrap(h2, n, c, b)), n(0) {
    a.reserve(_Nm), b.reserve(_Nm), c.reserve(_Nm);
  }
  const std::vector<T> &multiplicand() const { return a; }
  const std::vector<T> &multiplier() const { return b; }
  T at(int k) { return (*this)[k]; }
  T operator[](int k) {
    while (n <= k) next();
    return c[k];
  }
  T next() {
    using GNA1 = GlobalNTTArray<T, _Nm, 1>;
    using GNA2 = GlobalNTTArray<T, _Nm, 2>;
    static constexpr int BASE_CASE_SIZE = 32;
    if (int l = get_len(n << 1 | 1); (int)c.size() < l) c.resize(l);
    if (n == 0) c[0] = ha() * hb();
    if (n == 1) c[1] = ha() * b[0] + a[0] * hb(), c[2] = a[1] * b[1];
    if (n == 2)
      c[2] += ha() * b[0] + a[0] * hb(), c[3] = a[2] * b[1] + a[1] * b[2];
    if (n > 2) {
      if (!(n & (n - 1))) {
        int t0 = n >> 1, t1 = n;
        auto &c0 = ac.emplace_back(), &c1 = bc.emplace_back();
        c0.resize(t1), c0.set(a.data() + t0, 0, t0), c0.dft(0, t1);
        c1.resize(t1), c1.set(b.data() + t0, 0, t0), c1.dft(0, t1);
        GNA1::bf.mul(c0, c1, 0, t1), GNA1::bf.idft(0, t1);
        for (int i = t1 - 1; i--;) c[t1 + i] += GNA1::bf.get(i);
      }
      c[n] += ha() * b[0] + a[0] * hb(), c[n + 1] += a[1] * b[n] + a[n] * b[1];
      for (int t0 = 2, sft = 0, ofs = get_len(n + 1) >> 1, t = n + 1 - ofs;
           !(t & 1) && t0 < ofs; t0 <<= 1, sft++, t >>= 1)
        if (int m = n + 1 - t0, t1 = t0 << 1; t0 > BASE_CASE_SIZE) {
          GNA1::bf.set(a.data() + m, 0, t0), GNA1::bf.zeros(t0, t1);
          GNA2::bf.set(b.data() + m, 0, t0), GNA2::bf.zeros(t0, t1);
          GNA1::bf.dft(0, t1), GNA2::bf.dft(0, t1);
          GNA1::bf.mul(bc[sft], 0, t1), GNA2::bf.mul(ac[sft], 0, t1);
          GNA1::bf.add(GNA2::bf, 0, t1), GNA1::bf.idft(0, t1);
          for (int i = t1 - 1; i--;) c[n + 1 + i] += GNA1::bf.get(i);
        } else
          for (int i = t0; i--;)
            for (int j = t0; j--;)
              c[n + 1 + i + j] += a[m + i] * b[j + t0] + a[j + t0] * b[m + i];
    }
    return c[n++];
  }
};
template <class mod_t, std::size_t _Nm>
class FormalPowerSeries;
template <class mod_t, std::size_t _Nm>
FormalPowerSeries<mod_t, _Nm> deriv(const FormalPowerSeries<mod_t, _Nm> &fps);
template <class mod_t, std::size_t _Nm>
FormalPowerSeries<mod_t, _Nm> integ(const FormalPowerSeries<mod_t, _Nm> &fps);
template <class mod_t, std::size_t _Nm>
FormalPowerSeries<mod_t, _Nm> log(const FormalPowerSeries<mod_t, _Nm> &fps);
template <class mod_t, std::size_t _Nm>
FormalPowerSeries<mod_t, _Nm> exp(const FormalPowerSeries<mod_t, _Nm> &fps);
template <class mod_t, std::size_t _Nm>
FormalPowerSeries<mod_t, _Nm> pow(const FormalPowerSeries<mod_t, _Nm> &fps,
                                  std::uint64_t k);
template <class mod_t, std::size_t _Nm = 1 << 22>
class FormalPowerSeries {
  using F = std::function<mod_t(int)>;
  using FPS = FormalPowerSeries;
  F h_;

 public:
  class Resetter {
    std::shared_ptr<F> p_;

   public:
    Resetter() {}
    Resetter(std::shared_ptr<F> p) : p_(p) {}
    void set(const FPS &rhs) { *p_ = rhs.handle(); }
  };
  class Inde {  // indeterminate
    int p_;

   public:
    Inde(int p) : p_(p) {}
    Inde() : Inde(1) {}
    Inde operator^(int p) const { return Inde(p_ * p); }
    Inde operator*(const Inde &rhs) const { return Inde(p_ + rhs.p_); }
    int pow() const { return p_; }
  };
  FormalPowerSeries() : h_([](int) { return mod_t(0); }) {}
  FormalPowerSeries(F f)
      : h_([f, cache = std::make_shared<std::vector<mod_t>>()](int k) -> mod_t {
          for (int i = (int)cache->size(); i <= k; ++i)
            cache->emplace_back(f(i));
          return cache->at(k);
        }) {}
  FormalPowerSeries(const std::vector<mod_t> &coef)
      : h_([cache =
                std::make_shared<std::vector<mod_t>>(coef)](int k) -> mod_t {
          return k < (int)cache->size() ? cache->at(k) : mod_t(0);
        }) {}
  FormalPowerSeries(mod_t v)
      : h_([v](int k) { return k == 0 ? v : mod_t(0); }) {}
  F handle() const { return h_; }
  static Inde x() { return Inde(); }
  Resetter reset() {
    auto p = std::make_shared<F>();
    return h_ = [p](int i) { return (*p)(i); }, Resetter(p);
  }
  mod_t operator[](int k) const { return h_(k); }
  FPS operator()(const Inde &rhs) const { return scale(rhs.pow()); }
  FPS operator*(const Inde &rhs) const { return shift(rhs.pow()); }
  FPS operator*(const mod_t &rhs) const {
    return FPS([h = h_, v = rhs](int i) { return h(i) * v; });
  }
  FPS operator/(const mod_t &rhs) const {  // `rhs == 0` is not allowed
    return FPS([h = h_, v = mod_t(1) / rhs](int i) { return h(i) * v; });
  }
  FPS operator+(const mod_t &rhs) const {
    return FPS([h = h_, v = rhs](int i) { return i ? h(i) : h(i) + v; });
  }
  FPS operator-(const mod_t &rhs) const {
    return FPS([h = h_, v = rhs](int i) { return i ? h(i) : h(i) - v; });
  }
  friend FPS operator*(const Inde &lhs, const FPS &rhs) {
    return rhs.shift(lhs.pow());
  }
  friend FPS operator-(const mod_t &lhs, const FPS &rhs) {
    return FPS([h = rhs.h_, v = lhs](int i) { return i ? -h(i) : v - h(i); });
  }
  friend FPS operator+(const mod_t &lhs, const FPS &rhs) {
    return FPS([h = rhs.h_, v = lhs](int i) { return i ? h(i) : h(i) + v; });
  }
  friend FPS operator*(const mod_t &lhs, const FPS &rhs) {
    return FPS([h = rhs.h_, v = lhs](int i) { return h(i) * v; });
  }
  friend FPS operator/(const mod_t &lhs, const FPS &rhs) {
    return lhs * rhs.inv();
  }
  FPS scale(int k) const {
    return FPS([h = h_, k](int i) { return i % k ? mod_t(0) : h(i / k); });
  }
  FPS shift(int k) const {
    return FPS([h = h_, k](int i) { return i < k ? mod_t(0) : h(i - k); });
  }
  FPS inv() const {
    auto rc = std::make_shared<RelaxedConvolution<mod_t, _Nm>>(
        [h = h_](int i) { return h(i); },
        [h = h_, iv = mod_t()](int i, const auto &c) mutable {
          return i ? -(c[i] + h(i) * iv) * iv : (iv = mod_t(1) / h(0));
        });
    return FPS(
        [rc](int i) { return rc->next(), rc->multiplier()[i]; });  // safe
  }
  friend FPS deriv<mod_t, _Nm>(const FPS &fps);
  friend FPS integ<mod_t, _Nm>(const FPS &fps);
  friend FPS log<mod_t, _Nm>(const FPS &fps);
  friend FPS exp<mod_t, _Nm>(const FPS &fps);
  friend FPS pow<mod_t, _Nm>(const FPS &fps, std::uint64_t k);
  FPS operator+(const FPS &rhs) const {
    return FPS([h0 = h_, h1 = rhs.h_](int i) { return h0(i) + h1(i); });
  }
  FPS operator-(const FPS &rhs) const {
    return FPS([h0 = h_, h1 = rhs.h_](int i) { return h0(i) - h1(i); });
  }
  FPS operator-() const {
    return FPS([h = h_](int i) { return -h(i); });
  }
  FPS operator*(const FPS &rhs) const {
    auto rc = std::make_shared<RelaxedConvolution<mod_t, _Nm>>(
        [h = h_](int i) { return h(i); }, [h = rhs.h_](int i) { return h(i); });
    return FPS([rc](int) { return rc->next(); });
  }
  FPS operator/(const FPS &rhs) const {
    auto rc = std::make_shared<RelaxedConvolution<mod_t, _Nm>>(
        [h = rhs.h_](int i) { return h(i); },
        [h0 = h_, h1 = rhs.h_, iv = mod_t(), t0 = mod_t()](
            int i, const auto &c) mutable {
          if (i == 0) return t0 = h0(0) * (iv = mod_t(1) / h1(0));
          return (h0(i) - h1(i) * t0 - c[i]) * iv;
        });
    return FPS([rc](int i) { return rc->next(), rc->multiplier()[i]; });
  }
};
template <class mod_t, std::size_t _Nm>
FormalPowerSeries<mod_t, _Nm> deriv(const FormalPowerSeries<mod_t, _Nm> &fps) {
  return FormalPowerSeries<mod_t, _Nm>(
      [h = fps.h_](int i) { return h(i + 1) * mod_t(i + 1); });
}
template <class mod_t, std::size_t _Nm>
FormalPowerSeries<mod_t, _Nm> integ(const FormalPowerSeries<mod_t, _Nm> &fps) {
  return FormalPowerSeries<mod_t, _Nm>([h = fps.h_](int i) {
    return i ? h(i - 1) * get_inv<mod_t, _Nm>(i) : mod_t(0);
  });
}
template <class mod_t, std::size_t _Nm>
FormalPowerSeries<mod_t, _Nm> log(const FormalPowerSeries<mod_t, _Nm> &fps) {
  return assert(fps[0] == mod_t(1)), integ(deriv(fps) / fps);
}
template <class mod_t, std::size_t _Nm>
FormalPowerSeries<mod_t, _Nm> exp(const FormalPowerSeries<mod_t, _Nm> &fps) {
  assert(fps[0] == mod_t(0));
  auto rc = std::make_shared<RelaxedConvolution<mod_t, _Nm>>(
      [h = fps.h_](int i) { return h(i + 1) * mod_t(i + 1); },
      [](int i, const auto &c) {
        return i ? c[i - 1] * get_inv<mod_t, _Nm>(i) : mod_t(1);
      });
  return FormalPowerSeries<mod_t, _Nm>([rc](int i) {
    return i ? rc->at(i - 1) * get_inv<mod_t, _Nm>(i) : mod_t(1);
  });
}
template <class mod_t, std::size_t _Nm>
FormalPowerSeries<mod_t, _Nm> pow(const FormalPowerSeries<mod_t, _Nm> &fps,
                                  std::uint64_t k) {
  using FPS = FormalPowerSeries<mod_t, _Nm>;
  return FPS([h = fps.h_, kk = mod_t(k), k, cnt = 0ull,
              s = std::optional<std::function<mod_t(int)>>()](int i) mutable {
    if (s) return (unsigned long long)i < cnt ? mod_t(0) : (*s)(i - (int)cnt);
    mod_t v(h(i));
    if (v == mod_t(0)) return cnt++, mod_t(0);
    cnt *= k;
    FPS t0([os = i, iv = mod_t(1) / v, h](int i) { return h(i + os) * iv; });
    FPS t1([h0 = log(t0).handle(), kk](int i) { return h0(i) * kk; });
    s.emplace(
        [vk = v.pow(k), h1 = exp(t1).handle()](int i) { return h1(i) * vk; });
    return cnt ? mod_t(0) : (*s)(i);
  });
}