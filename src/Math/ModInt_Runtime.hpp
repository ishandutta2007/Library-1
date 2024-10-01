#pragma once
#include "src/Math/ModInt.hpp"
class Montgomery32 {};  // mod < 2^32 & mod is odd
class Montgomery64 {};  // mod < 2^62 & mod is odd
class Barrett {};       // 2^20 < mod <= 2^41
namespace math_internal {
struct r_b: m_b {};
}
template <class mod_t> constexpr bool is_runtimemodint_v= std::is_base_of_v<math_internal::r_b, mod_t>;
namespace math_internal {
template <class MP, u64 M, int id> struct RB: r_b {
 static inline void set_mod(u64 m) { assert(m <= M), md= MP(m); }
 static inline u64 max() { return M; }
protected:
 static inline MP md;
};
template <class T, typename= enable_if_t<is_runtimemodint_v<T>>> constexpr u64 mv() { return T::max(); }
template <class Int, int id= -1> using ModInt_Runtime= conditional_t<is_same_v<Int, int>, MInt<unsigned, RB<MP_Na, 0xFFFFFFFF, id>>, conditional_t<is_same_v<Int, unsigned>, MInt<unsigned, RB<MP_Na, 0xFFFFFFFF, id>>, conditional_t<is_same_v<Int, long long>, MInt<u64, RB<MP_D2B1_1, (1ull << 63) - 1, id>>, conditional_t<is_same_v<Int, Montgomery32>, MInt<unsigned, RB<MP_Mo32, 0xFFFFFFFF, id>>, conditional_t<is_same_v<Int, Montgomery64>, MInt<u64, RB<MP_Mo64, (1ull << 62) - 1, id>>, conditional_t<is_same_v<Int, Barrett>, MInt<u64, RB<MP_Br, 1ull << 41, id>>, MInt<u64, RB<MP_D2B1_2, uint64_t(-1), id>>>>>>>>;
}
using math_internal::ModInt_Runtime;