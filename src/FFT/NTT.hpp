#pragma once
#include <bits/stdc++.h>
#include "src/Math/is_prime.hpp"
#include "src/Math/ModInt.hpp"

/**
 * @title Number-Theoretic-Transform
 * @category FFT
 */

// BEGIN CUT HERE
namespace ntt_internal {
using u64 = std::uint64_t;
using u128 = __uint128_t;
template <class mod_t>
struct NumberTheoreticTransform {
  static inline void dft(int n, mod_t x[]) {
    for (int m = n, h = 0, i0 = 0; m >>= 1; h = 0, i0 = 0)
      for (mod_t prod = 1, u; i0 < n; prod *= r2[bsf(++h)], i0 += (m << 1))
        for (int i = i0; i < i0 + m; ++i)
          x[i + m] = x[i] - (u = prod * x[i + m]), x[i] += u;
  }
  static inline void idft(int n, mod_t x[]) {
    for (int m = 1, h = 0, i0 = 0; m < n; m <<= 1, h = 0, i0 = 0)
      for (mod_t prod = 1, y; i0 < n; prod *= ir2[bsf(++h)], i0 += (m << 1))
        for (int i = i0; i < i0 + m; ++i)
          y = x[i] - x[i + m], x[i] += x[i + m], x[i + m] = prod * y;
    for (const mod_t iv = mod_t(1) / n; n--;) x[n] *= iv;
  }
  static void even_dft(int n, mod_t x[]) {
    for (int i = 0, j = 0; i < n; i += 2, j++) x[j] = iv2 * (x[i] + x[i + 1]);
  }
  static void odd_dft(int n, mod_t x[]) {
    mod_t prod = iv2;
    for (int i = 0, j = 0; i < n; i += 2, j++)
      x[j] = prod * (x[i] - x[i + 1]), prod *= ir2[bsf(~((u64)j))];
  }
  static void dft_doubling(int n, mod_t x[]) {
    std::copy_n(x, n, x + n), idft(n, x + n);
    mod_t k(1), t(rt[bsf(n << 1)]);
    for (int i = 0; i < n; i++) x[n + i] *= k, k *= t;
    dft(n, x + n);
  }
  static constexpr std::uint64_t lim() { return 1ULL << E; }

 protected:
  static constexpr mod_t pow2th_root(std::uint8_t e) {
    for (mod_t r = 2;; r += 1)
      if (auto s = r.pow((mod_t::modulo() - 1) / 2); s != 1 && s * s == 1)
        return r.pow((mod_t::modulo() - 1) >> e);
    return 0;  // can not find
  }            // return ω (primitive 2^e th root)
  static_assert(mod_t::modulo() & 1);
  static_assert(is_prime(mod_t::modulo()));
  static constexpr std::uint8_t E = bsf(mod_t::modulo() - 1);
  static constexpr auto roots(mod_t w) {
    std::array<mod_t, E + 1> ret = {};
    for (std::uint8_t e = E; e; e--, w *= w) ret[e] = w;
    return ret[0] = w, ret;
  }
  static constexpr auto ratios(const std::array<mod_t, E + 1> &rt,
                               const std::array<mod_t, E + 1> &irt, int i = 2) {
    std::array<mod_t, E - 1> ret = {};
    for (mod_t prod = 1; i <= E; prod *= irt[i++]) ret[i - 2] = rt[i] * prod;
    return ret;
  }
  static constexpr mod_t w = pow2th_root(E), iw = w.pow(lim() - 1);
  static constexpr mod_t iv2 = mod_t((mod_t::modulo() + 1) / 2);
  static_assert(w != mod_t(0));
  static constexpr auto rt = roots(w), irt = roots(iw);
  static constexpr auto r2 = ratios(rt, irt), ir2 = ratios(irt, rt);
};
template <class T, class B>
struct NTTArrayImpl : public B {
  using B::B;
  static constexpr std::uint8_t type() { return B::type; }
#define FUNC(op, name, HOGEHOGE, ...)            \
  inline void name(__VA_ARGS__) {                \
    HOGEHOGE(op, 1);                             \
    if constexpr (B::type >= 2) HOGEHOGE(op, 2); \
    if constexpr (B::type >= 3) HOGEHOGE(op, 3); \
  }
#define DFT(fft, _) B::ntt##_::fft(e - b, this->dat##_ + b)
#define ZEROS(op, _) std::fill_n(this->dat##_ + b, e - b, B::Z##_)
#define SET(op, _) std::copy(x + b, x + e, this->dat##_ + b)
#define SUBST(op, _) std::copy(r.dat##_ + b, r.dat##_ + e, this->dat##_ + b)
  FUNC(dft, dft, DFT, int b, int e)
  FUNC(idft, idft, DFT, int b, int e)
  FUNC(__, zeros, ZEROS, int b, int e)
  FUNC(__, set, SET, const T x[], int b, int e)
  template <class C>
  FUNC(__, subst, SUBST, const NTTArrayImpl<T, C> &r, int b, int e)
  inline void get(T x[], int b, int e) const {
    if constexpr (B::type == 1)
      std::copy(this->dat1 + b, this->dat1 + e, x + b);
    else
      for (int i = b; i < e; i++) x[i] = get(i);
  }
  inline T get(int i) const {
    if constexpr (B::type == 3) {
      const T mod1 = B::mint1::modulo(), mod2 = B::mint2::modulo();
      u64 r1 = this->dat1[i].val(), r2 = (B::iv21 * (this->dat2[i] - r1)).val();
      u64 r3 = (B::iv31 * (this->dat3[i] - r1) - B::iv32 * r2).val();
      return mod1 * (mod2 * r3 + r2) + r1;
    } else if constexpr (B::type == 2) {
      const T mod1 = B::mint1::modulo();
      u64 r1 = this->dat1[i].val();
      return mod1 * ((this->dat2[i] - r1) * B::iv).val() + r1;
    } else
      return this->dat1[i];
  }
#define ASGN(op, _) \
  for (int i = b; i < e; i++) this->dat##_[i] op## = r.dat##_[i]
#define ASSIGN(fname, op) \
  template <class C>      \
  FUNC(op, fname, ASGN, const NTTArrayImpl<T, C> &r, int b, int e)
#define BOP(op, _) \
  for (int i = b; i < e; i++) this->dat##_[i] = l.dat##_[i] op r.dat##_[i]
#define OP(fname, op)                               \
  template <class C, class D>                       \
  FUNC(op, fname, BOP, const NTTArrayImpl<T, C> &l, \
       const NTTArrayImpl<T, D> &r, int b, int e)
  OP(add, +) OP(dif, -) OP(mul, *) ASSIGN(add, +) ASSIGN(dif, -) ASSIGN(mul, *)
#undef DFT
#undef ZEROS
#undef SET
#undef SUBST
#undef ASGN
#undef ASSIGN
#undef BOP
#undef OP
#undef FUNC
};
template <class T, std::size_t _Nm>
struct NTTArrayB_SingleB {
  using ntt1 = NumberTheoreticTransform<T>;
  static_assert(_Nm <= ntt1::lim());
  static constexpr T Z1 = 0;
  static constexpr std::uint8_t type = 1;
};
template <class T, std::size_t _Nm, bool is_heap>
struct NTTArrayB_Single : protected NTTArrayB_SingleB<T, _Nm> {
  T dat1[_Nm] = {};
};
template <class T, std::size_t _Nm>
struct NTTArrayB_Single<T, _Nm, true> : protected NTTArrayB_SingleB<T, _Nm> {
  NTTArrayB_Single() : dat1(buf1.data()) {}
  void resize(int n) {
    buf1.resize(n, NTTArrayB_Single::Z1), dat1 = buf1.data();
  }
  std::size_t size() const { return buf1.size(); }
  std::vector<T> buf1;
  T *dat1;
};
#define NTTARRAYB_MULTI(iv, t)                                    \
  using mint1 = StaticModInt<MOD1>;                               \
  using mint2 = StaticModInt<MOD2>;                               \
  using ntt1 = NumberTheoreticTransform<mint1>;                   \
  using ntt2 = NumberTheoreticTransform<mint2>;                   \
  static_assert(_Nm <= (1 << 25));                                \
  static constexpr mint1 Z1 = 0;                                  \
  static constexpr mint2 iv = mint2(1) / mint1::modulo(), Z2 = 0; \
  static constexpr std::uint8_t type = t;
template <std::size_t _Nm, u64 MOD1, u64 MOD2>
struct NTTArrayB_DoubleB {
  NTTARRAYB_MULTI(iv, 2);
};
template <std::size_t _Nm, u64 MOD1, u64 MOD2, u64 MOD3>
struct NTTArrayB_TripleB {
  NTTARRAYB_MULTI(iv21, 3);
  using mint3 = StaticModInt<MOD3>;
  using ntt3 = NumberTheoreticTransform<mint3>;
  static constexpr mint3 iv32 = mint3(1) / mint2::modulo(),
                         iv31 = iv32 / mint1::modulo(), Z3 = 0;
};
#undef NTTARRAYB_MULTI
template <std::size_t _Nm, u64 MOD1, u64 MOD2, bool is_heap>
struct NTTArrayB_Double : protected NTTArrayB_DoubleB<_Nm, MOD1, MOD2> {
  typename NTTArrayB_DoubleB<_Nm, MOD1, MOD2>::mint1 dat1[_Nm] = {};
  typename NTTArrayB_DoubleB<_Nm, MOD1, MOD2>::mint2 dat2[_Nm] = {};
};
template <std::size_t _Nm, u64 MOD1, u64 MOD2>
struct NTTArrayB_Double<_Nm, MOD1, MOD2, true>
    : protected NTTArrayB_DoubleB<_Nm, MOD1, MOD2> {
  NTTArrayB_Double() : dat1(buf1.data()), dat2(buf2.data()) {}
  void resize(int n) {
    buf1.resize(n, NTTArrayB_DoubleB<_Nm, MOD1, MOD2>::Z1), dat1 = buf1.data();
    buf2.resize(n, NTTArrayB_DoubleB<_Nm, MOD1, MOD2>::Z2), dat2 = buf2.data();
  }
  std::size_t size() const { return buf1.size(); }
  std::vector<typename NTTArrayB_DoubleB<_Nm, MOD1, MOD2>::mint1> buf1;
  std::vector<typename NTTArrayB_DoubleB<_Nm, MOD1, MOD2>::mint2> buf2;
  typename NTTArrayB_DoubleB<_Nm, MOD1, MOD2>::mint1 *dat1;
  typename NTTArrayB_DoubleB<_Nm, MOD1, MOD2>::mint2 *dat2;
};
template <std::size_t _Nm, u64 MOD1, u64 MOD2, u64 MOD3, bool is_heap>
struct NTTArrayB_Triple : protected NTTArrayB_TripleB<_Nm, MOD1, MOD2, MOD3> {
  typename NTTArrayB_TripleB<_Nm, MOD1, MOD2, MOD3>::mint1 dat1[_Nm] = {};
  typename NTTArrayB_TripleB<_Nm, MOD1, MOD2, MOD3>::mint2 dat2[_Nm] = {};
  typename NTTArrayB_TripleB<_Nm, MOD1, MOD2, MOD3>::mint3 dat3[_Nm] = {};
};
template <std::size_t _Nm, u64 MOD1, u64 MOD2, u64 MOD3>
struct NTTArrayB_Triple<_Nm, MOD1, MOD2, MOD3, true>
    : protected NTTArrayB_TripleB<_Nm, MOD1, MOD2, MOD3> {
  NTTArrayB_Triple()
      : dat1(buf1.data()), dat2(buf2.data()), dat3(buf3.data()) {}
  void resize(int n) {
    buf1.resize(n, NTTArrayB_TripleB<_Nm, MOD1, MOD2, MOD3>::Z1);
    buf2.resize(n, NTTArrayB_TripleB<_Nm, MOD1, MOD2, MOD3>::Z2);
    buf3.resize(n, NTTArrayB_TripleB<_Nm, MOD1, MOD2, MOD3>::Z3);
    dat1 = buf1.data(), dat2 = buf2.data(), dat3 = buf3.data();
  }
  std::size_t size() const { return buf1.size(); }
  std::vector<typename NTTArrayB_TripleB<_Nm, MOD1, MOD2, MOD3>::mint1> buf1;
  std::vector<typename NTTArrayB_TripleB<_Nm, MOD1, MOD2, MOD3>::mint2> buf2;
  std::vector<typename NTTArrayB_TripleB<_Nm, MOD1, MOD2, MOD3>::mint3> buf3;
  typename NTTArrayB_TripleB<_Nm, MOD1, MOD2, MOD3>::mint1 *dat1;
  typename NTTArrayB_TripleB<_Nm, MOD1, MOD2, MOD3>::mint2 *dat2;
  typename NTTArrayB_TripleB<_Nm, MOD1, MOD2, MOD3>::mint3 *dat3;
};
template <class T, std::size_t _Nm>
constexpr bool is_nttfriend() {
  if constexpr (!is_staticmodint_v<T>)
    return false;
  else
    return (T::modulo() & is_prime(T::modulo())) &&
           _Nm <= (1ULL << bsf(T::modulo() - 1));
}
constexpr std::uint32_t MOD32 = 0x7e000001;
constexpr u64 MOD64_1 = 0x3ffffffffa000001, MOD64_2 = 0x3fffffffea000001,
              MOD64_3 = 0x3fffffffcc000001;
constexpr bool threshold(u64 val, u64 s, u64 m) {
  return u128(val) * val <= u128(MOD64_1) * m / s;
}
template <class T>
constexpr u64 max_value() {
  if constexpr (is_dynamicmodint_v<T>)
    return std::numeric_limits<typename T::Uint>::max();
  else if constexpr (is_staticmodint_v<T>)
    return T::modulo();
  else
    return std::numeric_limits<T>::max();
}
template <class T, std::size_t _Nm>
constexpr bool is_nttarraydouble = threshold(max_value<T>(), _Nm, MOD32);
template <class T, std::size_t _Nm, bool is_heap>
using NTTArrayB_Multi =
    std::conditional_t<is_nttarraydouble<T, _Nm>,
                       NTTArrayB_Double<_Nm, MOD64_1, MOD32, is_heap>,
                       NTTArrayB_Triple<_Nm, MOD64_1, MOD64_2, MOD32, is_heap>>;
template <class T, std::size_t _Nm, bool is_heap>
using NTTArrayB = std::conditional_t<is_nttfriend<T, _Nm>(),
                                     NTTArrayB_Single<T, _Nm, is_heap>,
                                     NTTArrayB_Multi<T, _Nm, is_heap>>;
template <class T, std::size_t _Nm, bool is_heap = false>
using NTTArray = NTTArrayImpl<T, NTTArrayB<T, _Nm, is_heap>>;
}  // namespace ntt_internal
using ntt_internal::is_nttarraydouble, ntt_internal::is_nttfriend,
    ntt_internal::NumberTheoreticTransform, ntt_internal::NTTArray;
template <class T, std::size_t _Nm>
constexpr int nttarray_type = NTTArray<T, _Nm>::type();
template <class T, std::size_t _Nm, int id = 0>
struct GlobalNTTArray {
  static inline NTTArray<T, _Nm, false> bf;
};
template <class T, std::size_t _Nm, std::size_t _Nm2, int id = 0>
struct GlobalNTTArray2D {
  static inline NTTArray<T, _Nm, false> bf[_Nm2];
};
template <class T, std::size_t _Nm, int id = 0>
struct GlobalArray {
  static inline T bf[_Nm];
};
constexpr std::uint32_t get_len(std::uint32_t n) {
  return (n |= (n |= (n |= (n |= (n |= (--n) >> 1) >> 2) >> 4) >> 8) >> 16) + 1;
}