#pragma once
#include <bits/stdc++.h>
#include "src/Math/is_prime.hpp"
#include "src/Math/ModInt.hpp"
namespace math_internal {
template <class mod_t> struct NumberTheoreticTransform {
  static inline void dft(int n, mod_t x[]) {
    if constexpr (mod < INT_MAX) {
      static constexpr auto r3= ratios<3>(rt, irt);
      static constexpr u128 imag= rt[2].val(), mod3= u128(mod) << 93;
      for (int p= n >> 2, ed= 1; p; p>>= 2, ed<<= 2) {
        mod_t rot= one, rot2= one, rot3= one, *x0= x, *x1, *x2, *x3;
        for (int s= 0, i;; rot*= r3[__builtin_ctz(s)], rot2= rot * rot,
                 rot3= rot2 * rot, x0= x3 + p) {
          u64 rot_u= rot.val(), rot2_u= rot2.val(), rot3_u= rot3.val();
          for (x1= x0 + p, x2= x1 + p, x3= x2 + p, i= p; i--;) {
            u64 a0= x0[i].val(), a1= rot_u * x1[i].val(),
                a2= rot2_u * x2[i].val(), a3= rot3_u * x3[i].val(),
                na3= mod2 - a3, a0n2= mod2 + a0 - a2;
            u128 a1na3imag= imag * (a1 + na3);
            x0[i]= a0 + a1 + a2 + a3, x1[i]= a0 + (mod2 - a1) + a2 + na3;
            x2[i]= a1na3imag + a0n2, x3[i]= mod3 - a1na3imag + a0n2;
          }
          if (++s == ed) break;
        }
      }
      if (__builtin_ctz(n) & 1) {
        mod_t rot= one, u;
        for (int s= 0, m= 0;; rot*= r2[__builtin_ctz(++s)]) {
          x[m + 1]= x[m] - (u= x[m + 1] * rot), x[m]+= u;
          if ((m+= 2) == n) break;
        }
      }
    } else
      for (int m= n, s= 0, i, ed= 1; m>>= 1; s= 0, ed<<= 1)
        for (mod_t rot= one, *x0= x, *x1, u;;
             rot*= r2[__builtin_ctz(s)], x0= x1 + m) {
          for (x1= x0 + (i= m); i--;)
            x1[i]= x0[i] - (u= rot * x1[i]), x0[i]+= u;
          if (++s == ed) break;
        }
  }
  static inline void idft(int n, mod_t x[]) {
    if constexpr (mod < INT_MAX) {
      static constexpr auto ir3= ratios<3>(irt, rt);
      static constexpr u64 iimag= irt[2].val();
      for (int ed= n >> 2, p= 1; ed; p<<= 2, ed>>= 2) {
        mod_t irot= one, irot2= one, irot3= one, *x0= x, *x1, *x2, *x3;
        for (int s= 0, i;; irot*= ir3[__builtin_ctz(s)], irot2= irot * irot,
                 irot3= irot2 * irot, x0= x3 + p) {
          u64 irot2_u= irot2.val();
          u128 irot_u= irot.val(), irot3_u= irot3.val();
          for (x1= x0 + p, x2= x1 + p, x3= x2 + p, i= p; i--;) {
            u64 a0= x0[i].val(), a1= x1[i].val(), a2= x2[i].val(),
                a3= x3[i].val(), na3= mod - a3, a0n1= a0 + mod - a1,
                a2na3iimag= iimag * (a2 + na3);
            x0[i]= a0 + a1 + a2 + a3, x1[i]= irot_u * (a0n1 + a2na3iimag);
            x2[i]= irot2_u * (a0 + a1 + (mod - a2) + na3);
            x3[i]= irot3_u * (a0n1 + (mod2 << 1) - a2na3iimag);
          }
          if (++s == ed) break;
        }
      }
      if (__builtin_ctz(n) & 1) {
        mod_t u, *x1= x + (n >> 1);
        for (int i= n >> 1; i--;) u= x[i] - x1[i], x[i]+= x1[i], x1[i]= u;
      }
    } else
      for (int m= 1, s= 0, i, ed= n; ed>>= 1; m<<= 1, s= 0)
        for (mod_t irot= one, y, *x0= x, *x1;;
             irot*= ir2[__builtin_ctz(s)], x0= x1 + m) {
          for (x1= x0 + (i= m); i--;)
            y= x0[i] - x1[i], x0[i]+= x1[i], x1[i]= irot * y;
          if (++s == ed) break;
        }
    for (const mod_t iv= one / n; n--;) x[n]*= iv;
  }
  static inline void even_dft(int n, mod_t x[]) {
    for (int i= 0, j= 0; i < n; i+= 2, j++) x[j]= iv2 * (x[i] + x[i + 1]);
  }
  static inline void odd_dft(int n, mod_t x[]) {
    mod_t prod= iv2;
    for (int i= 0, j= 0;; i+= 2, prod*= ir2[__builtin_ctz(++j)])
      if (x[j]= prod * (x[i] - x[i + 1]); i + 2 == n) break;
  }
  static inline void dft_doubling(int n, mod_t x[]) {
    copy_n(x, n, x + n), idft(n, x + n);
    mod_t k(1), t(rt[__builtin_ctz(n << 1)]);
    for (int i= 0; i < n; i++) x[n + i]*= k, k*= t;
    dft(n, x + n);
  }
  static constexpr u64 lim() { return 1ULL << E; }
 protected:
  static constexpr u64 mod= mod_t::modulo(), mod2= mod << 31;
  static_assert(mod & 1);
  static_assert(is_prime(mod));
  static constexpr uint8_t E= __builtin_ctzll(mod - 1);
  static constexpr mod_t w= [](uint8_t e) -> mod_t {
    for (mod_t r= 2;; r+= 1)
      if (auto s= r.pow((mod - 1) / 2); s != 1 && s * s == 1)
        return r.pow((mod - 1) >> e);
    return 0;
  }(E);
  static_assert(w != mod_t(0));
  static constexpr mod_t one= 1, iv2= (mod + 1) / 2, iw= w.pow(lim() - 1);
  static constexpr auto roots(mod_t w) {
    array<mod_t, E + 1> ret= {};
    for (uint8_t e= E; e; e--, w*= w) ret[e]= w;
    return ret[0]= w, ret;
  }
  template <size_t N>
  static constexpr auto ratios(const array<mod_t, E + 1> &rt,
                               const array<mod_t, E + 1> &irt, int i= N) {
    array<mod_t, E + 1 - N> ret= {};
    for (mod_t prod= 1; i <= E; prod*= irt[i++]) ret[i - N]= rt[i] * prod;
    return ret;
  }
  static constexpr auto rt= roots(w), irt= roots(iw);
  static constexpr auto r2= ratios<2>(rt, irt), ir2= ratios<2>(irt, rt);
};
template <class T, uint8_t type, class B> struct NTTArrayImpl: public B {
  using B::B;
#define FUNC(op, name, HOGEHOGE, ...)         \
  inline void name(__VA_ARGS__) {             \
    HOGEHOGE(op, 1);                          \
    if constexpr (type >= 2) HOGEHOGE(op, 2); \
    if constexpr (type >= 3) HOGEHOGE(op, 3); \
    if constexpr (type >= 4) HOGEHOGE(op, 4); \
    if constexpr (type >= 5) HOGEHOGE(op, 5); \
  }
#define DFT(fft, _) B::ntt##_::fft(e - b, this->dat##_ + b)
#define ZEROS(op, _) fill_n(this->dat##_ + b, e - b, typename B::mint##_())
#define SET(op, _) copy(x + b, x + e, this->dat##_ + b)
#define SET_SINGLE(op, _) this->dat##_[i]= x;
#define SUBST(op, _) copy(r.dat##_ + b, r.dat##_ + e, this->dat##_ + b)
  FUNC(dft, dft, DFT, int b, int e)
  FUNC(idft, idft, DFT, int b, int e)
  FUNC(__, zeros, ZEROS, int b, int e)
  FUNC(__, set, SET, const T x[], int b, int e)
  FUNC(__, set, SET_SINGLE, int i, T x)
  template <class C>
  FUNC(__, subst, SUBST, const NTTArrayImpl<T, type, C> &r, int b, int e)
  inline void get(T x[], int b, int e) const {
    if constexpr (type == 1) copy(this->dat1 + b, this->dat1 + e, x + b);
    else
      for (int i= b; i < e; i++) x[i]= get(i);
  }
#define TMP(num) B::iv##num##1 * (this->dat##num[i] - r1)
  inline T get(int i) const {
    if constexpr (type >= 2) {
      static const T mod1= B::mint1::modulo();
      u64 r1= this->dat1[i].val(), r2= (TMP(2)).val();
      T ret= 0;
      if constexpr (type >= 3) {
        static const T mod2= B::mint2::modulo();
        u64 r3= (TMP(3) - B::iv32 * r2).val();
        if constexpr (type >= 4) {
          static const T mod3= B::mint3::modulo();
          u64 r4= (TMP(4) - B::iv42 * r2 - B::iv43 * r3).val();
          if constexpr (type >= 5) {
            static const T mod4= B::mint4::modulo();
            u64 r5= (TMP(5) - B::iv52 * r2 - B::iv53 * r3 - B::iv54 * r4).val();
            ret= mod4 * r5;
          }
          ret= mod3 * (ret + r4);
        }
        ret= mod2 * (ret + r3);
      }
      return mod1 * (ret + r2) + r1;
    } else return this->dat1[i];
  }
#undef TMP
#define ASGN(op, _) \
  for (int i= b; i < e; i++) this->dat##_[i] op##= r.dat##_[i]
#define ASSIGN(fname, op) \
  template <class C>      \
  FUNC(op, fname, ASGN, const NTTArrayImpl<T, type, C> &r, int b, int e)
#define BOP(op, _) \
  for (int i= b; i < e; i++) this->dat##_[i]= l.dat##_[i] op r.dat##_[i]
#define OP(fname, op)                                     \
  template <class C, class D>                             \
  FUNC(op, fname, BOP, const NTTArrayImpl<T, type, C> &l, \
       const NTTArrayImpl<T, type, D> &r, int b, int e)
  OP(add, +)
  OP(dif, -) OP(mul, *) ASSIGN(add, +) ASSIGN(dif, -) ASSIGN(mul, *)
#undef DFT
#undef ZEROS
#undef SET
#undef SET_SINGLE
#undef SUBST
#undef ASGN
#undef ASSIGN
#undef BOP
#undef OP
#undef FUNC
};
using u8= uint8_t;
#define ARR(num)                                       \
  using mint##num= StaticModInt<M##num>;               \
  using ntt##num= NumberTheoreticTransform<mint##num>; \
  mint##num dat##num[LIM]= {};
#define IV2 static constexpr mint2 iv21= mint2(1) / mint1::modulo();
#define IV3                                                \
  static constexpr mint3 iv32= mint3(1) / mint2::modulo(), \
                         iv31= iv32 / mint1::modulo();
#define IV4                                                \
  static constexpr mint4 iv43= mint4(1) / mint3::modulo(), \
                         iv42= iv43 / mint2::modulo(),     \
                         iv41= iv42 / mint1::modulo();
#define IV5                                                \
  static constexpr mint5 iv54= mint5(1) / mint4::modulo(), \
                         iv53= iv54 / mint3::modulo(),     \
                         iv52= iv53 / mint2::modulo(),     \
                         iv51= iv52 / mint1::modulo();
template <u8 type, u64 M1, u32 M2, u32 M3, u32 M4, u32 M5, size_t LIM, bool vec>
struct NTTArrayB_ {
  ARR(1)
};
template <u64 M1, u32 M2, u32 M3, u32 M4, u32 M5, size_t LIM>
struct NTTArrayB_<2, M1, M2, M3, M4, M5, LIM, false> {
  ARR(1) ARR(2) IV2
};
template <u64 M1, u32 M2, u32 M3, u32 M4, u32 M5, size_t LIM>
struct NTTArrayB_<3, M1, M2, M3, M4, M5, LIM, false> {
  ARR(1) ARR(2) ARR(3) IV2 IV3
};
template <u64 M1, u32 M2, u32 M3, u32 M4, u32 M5, size_t LIM>
struct NTTArrayB_<4, M1, M2, M3, M4, M5, LIM, false> {
  ARR(1) ARR(2) ARR(3) ARR(4) IV2 IV3 IV4
};
template <u64 M1, u32 M2, u32 M3, u32 M4, u32 M5, size_t LIM>
struct NTTArrayB_<5, M1, M2, M3, M4, M5, LIM, false> {
  ARR(1) ARR(2) ARR(3) ARR(4) ARR(5) IV2 IV3 IV4 IV5
};
#undef ARR
#define VEC(num)                                       \
  using mint##num= StaticModInt<M##num>;               \
  using ntt##num= NumberTheoreticTransform<mint##num>; \
  vector<mint##num> buf##num;                          \
  mint##num *dat##num;
template <u64 M1, u32 M2, u32 M3, u32 M4, u32 M5, size_t LIM>
struct NTTArrayB_<1, M1, M2, M3, M4, M5, LIM, true> {
  NTTArrayB_(): dat1(buf1.data()) {}
  void resize(int n) { buf1.resize(n), dat1= buf1.data(); }
  size_t size() const { return buf1.size(); }
  VEC(1)
};
template <u64 M1, u32 M2, u32 M3, u32 M4, u32 M5, size_t LIM>
struct NTTArrayB_<2, M1, M2, M3, M4, M5, LIM, true> {
  NTTArrayB_(): dat1(buf1.data()), dat2(buf2.data()) {}
  void resize(int n) {
    buf1.resize(n), dat1= buf1.data(), buf2.resize(n), dat2= buf2.data();
  }
  size_t size() const { return buf1.size(); }
  VEC(1) VEC(2) IV2
};
template <u64 M1, u32 M2, u32 M3, u32 M4, u32 M5, size_t LIM>
struct NTTArrayB_<3, M1, M2, M3, M4, M5, LIM, true> {
  NTTArrayB_(): dat1(buf1.data()), dat2(buf2.data()), dat3(buf3.data()) {}
  void resize(int n) {
    buf1.resize(n), dat1= buf1.data(), buf2.resize(n), dat2= buf2.data();
    buf3.resize(n), dat3= buf3.data();
  }
  size_t size() const { return buf1.size(); }
  VEC(1) VEC(2) VEC(3) IV2 IV3
};
template <u64 M1, u32 M2, u32 M3, u32 M4, u32 M5, size_t LIM>
struct NTTArrayB_<4, M1, M2, M3, M4, M5, LIM, true> {
  NTTArrayB_()
      : dat1(buf1.data()),
        dat2(buf2.data()),
        dat3(buf3.data()),
        dat4(buf4.data()) {}
  void resize(int n) {
    buf1.resize(n), dat1= buf1.data(), buf2.resize(n), dat2= buf2.data();
    buf3.resize(n), dat3= buf3.data(), buf4.resize(n), dat4= buf4.data();
  }
  size_t size() const { return buf1.size(); }
  VEC(1) VEC(2) VEC(3) VEC(4) IV2 IV3 IV4
};
template <u64 M1, u32 M2, u32 M3, u32 M4, u32 M5, size_t LIM>
struct NTTArrayB_<5, M1, M2, M3, M4, M5, LIM, true> {
  NTTArrayB_()
      : dat1(buf1.data()),
        dat2(buf2.data()),
        dat3(buf3.data()),
        dat4(buf4.data()),
        dat5(buf5.data()) {}
  void resize(int n) {
    buf1.resize(n), dat1= buf1.data(), buf2.resize(n), dat2= buf2.data();
    buf3.resize(n), dat3= buf3.data(), buf4.resize(n), dat4= buf4.data();
    buf5.resize(n), dat5= buf5.data();
  }
  size_t size() const { return buf1.size(); }
  VEC(1) VEC(2) VEC(3) VEC(4) VEC(5) IV2 IV3 IV4 IV5
};
#undef VEC
#undef IV2
#undef IV3
#undef IV4
#undef IV5
template <class T, size_t LIM> constexpr bool is_nttfriend() {
  if constexpr (!is_staticmodint_v<T>) return false;
  else
    return (T::modulo() & is_prime(T::modulo())) &&
           LIM <= (1ULL << __builtin_ctzll(T::modulo() - 1));
}
template <class T> constexpr u64 max_value() {
  if constexpr (is_runtimemodint_v<T>)
    return numeric_limits<typename T::Uint>::max();
  else if constexpr (is_staticmodint_v<T>) return T::modulo();
  else return numeric_limits<T>::max();
}
template <class T, size_t LIM, u32 M1, u32 M2, u32 M3, u32 M4>
constexpr uint8_t nttarray_type_() {
  if constexpr (is_nttfriend<T, LIM>()) return 1;
  else {
    constexpr u128 mv= max_value<T>(), mvmv= mv * mv;
    if constexpr (mvmv <= M1 / LIM) return 1;
    else if constexpr (mvmv <= u64(M1) * M2 / LIM) return 2;
    else if constexpr (mvmv <= u128(M1) * M2 * M3 / LIM) return 3;
    else if constexpr (mvmv <= u128(M1) * M2 * M3 * M4 / LIM) return 4;
    else return 5;
  }
}
constexpr std::uint32_t MOD32_1= 0x7e000001, MOD32_2= 0x78000001,
                        MOD32_3= 0x6c000001, MOD32_4= 0x66000001,
                        MOD32_5= 0x42000001;
template <class T, size_t LIM>
constexpr u8 nttarray_type=
    nttarray_type_<T, LIM, MOD32_1, MOD32_2, MOD32_3, MOD32_4>();
template <class T, size_t LIM>
constexpr u8 nttarr_cat= is_nttfriend<T, LIM>() && (max_value<T>() > INT_MAX)
                             ? 0
                             : nttarray_type<T, LIM>;
template <class T, size_t LIM, bool vec>
using NTTArrayB=
    conditional_t<is_nttfriend<T, LIM>(),
                  NTTArrayB_<1, max_value<T>(), 0, 0, 0, 0, LIM, vec>,
                  NTTArrayB_<nttarray_type<T, LIM>, MOD32_1, MOD32_2, MOD32_3,
                             MOD32_4, MOD32_5, LIM, vec>>;
template <class T, size_t LIM, bool vec>
using NTTArray= NTTArrayImpl<T, nttarray_type<T, LIM>, NTTArrayB<T, LIM, vec>>;
}
using math_internal::is_nttfriend, math_internal::nttarray_type,
    math_internal::nttarr_cat, math_internal::NumberTheoreticTransform,
    math_internal::NTTArray;
template <class T, std::size_t LIM, int id= 0> struct GlobalNTTArray {
  static inline NTTArray<T, LIM, false> bf;
};
template <class T, std::size_t LIM, std::size_t LIM2, int id= 0>
struct GlobalNTTArray2D {
  static inline NTTArray<T, LIM, false> bf[LIM2];
};
template <class T, std::size_t LIM, int id= 0> struct GlobalArray {
  static inline T bf[LIM];
};
constexpr unsigned get_len(unsigned n) { return 1 << (std::__lg(n - 1) + 1); }