#pragma once
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
namespace math_internal {
struct r_b: m_b {};
template <class mod_t> constexpr bool is_runtimemodint_v= is_base_of_v<r_b, mod_t>;
template <class MP, u64 M, int id> struct RB: r_b {
 static inline void set_mod(u64 m) { md= MP(m); }
 static inline u64 max() { return M; }
protected:
 static inline MP md;
};
class Montgomery32 {};
class Montgomery64 {};
class Barrett {};
class Barrett2 {};
template <class Int, int id= -1> using ModInt_Runtime= conditional_t<is_same_v<Int, Montgomery32>, MInt<int, u32, RB<MP_Mo<u32, u64, 32, 31>, (1 << 30), id>>, conditional_t<is_same_v<Int, Montgomery64>, MInt<i64, u64, RB<MP_Mo<u64, u128, 64, 63>, (1ull << 62), id>>, conditional_t<is_same_v<Int, Barrett>, MInt<int, u32, RB<MP_Br, (1u << 31), id>>, conditional_t<is_same_v<Int, Barrett2>, MInt<i64, u64, RB<MP_Br2, (1ull << 41), id>>, conditional_t<disjunction_v<is_same<Int, i64>, is_same<Int, u64>>, MInt<i64, u64, RB<MP_D2B1, ULLONG_MAX, id>>, MInt<int, u32, RB<MP_Na, UINT_MAX, id>>>>>>>;
template <class T, enable_if_t<is_runtimemodint_v<T>, nullptr_t> = nullptr> constexpr u64 mv() { return T::max(); }
}
using math_internal::ModInt_Runtime, math_internal::Montgomery32, math_internal::Montgomery64, math_internal::Barrett, math_internal::Barrett2, math_internal::is_runtimemodint_v;
