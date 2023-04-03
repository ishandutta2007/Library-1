#pragma once
#include <limits>
#include <type_traits>
#include <cstddef>
template <class M> struct SemiRing {
 using T= typename M::T;
 T x;
 SemiRing(): x(M::o) {}
 SemiRing(bool y): x(y ? M::i : M::o) {}
 template <class U, std::enable_if_t<std::is_convertible_v<U, T>, std::nullptr_t> = nullptr> SemiRing(U y): x((T)y) {}
 SemiRing &operator+=(const SemiRing &r) { return M::add(x, r.x), *this; }
 SemiRing &operator*=(const SemiRing &r) { return x == M::o || r.x == M::o ? *this= M::o : (M::mul(x, r.x), *this); }
 SemiRing operator+(const SemiRing &r) const { return SemiRing(*this)+= r; }
 SemiRing operator*(const SemiRing &r) const { return SemiRing(*this)*= r; }
 bool operator==(const SemiRing &r) const { return x == r.x; }
 bool operator!=(const SemiRing &r) const { return x != r.x; }
 friend std::istream &operator>>(std::istream &is, SemiRing &r) { return is >> r.x, is; }
 friend std::ostream &operator<<(std::ostream &os, const SemiRing &r) { return os << r.x; }
};
template <class Arith> struct MinPlus {
 using T= Arith;
 static inline T o= std::numeric_limits<T>::max(), i= T();
 static void add(T &vl, T vr) { vl= std::min(vl, vr); }
 static void mul(T &vl, T vr) { vl+= vr; }
};
template <class T> using MinPlusRig= Rig<MinPlus<T>>;
template <class Arith> struct MaxPlus {
 using T= Arith;
 static inline T o= std::numeric_limits<T>::min(), i= T();
 static void add(T &vl, T vr) { vl= std::max(vl, vr); }
 static void mul(T &vl, T vr) { vl+= vr; }
};
template <class T> using MaxPlusRig= Rig<MaxPlus<T>>;
template <class Arith> struct MinMax {
 using T= Arith;
 static inline T o= std::numeric_limits<T>::max(), i= std::numeric_limits<T>::min();
 static void add(T &vl, T vr) { vl= std::min(vl, vr); }
 static void mul(T &vl, T vr) { vl= std::max(vl, vr); }
};
template <class T> using MinMaxRig= Rig<MinMax<T>>;
template <class Uint> struct BitwiseOrAnd {
 using T= Uint;
 static constexpr T o= 0, i= T(-1);
 static void add(T &vl, T vr) { vl|= vr; }
 static void mul(T &vl, T vr) { vl&= vr; }
};
template <class T= unsigned long long> using BitwiseOrAndRig= Rig<BitwiseOrAnd<T>>;
template <class Uint> struct BitwiseXorAnd {
 using T= Uint;
 static constexpr T o= 0, i= T(-1);
 static void add(T &vl, T vr) { vl^= vr; }
 static void mul(T &vl, T vr) { vl&= vr; }
};
template <class T= unsigned long long> using BitwiseXorAndRig= Rig<BitwiseXorAnd<T>>;
