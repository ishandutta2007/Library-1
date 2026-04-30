#pragma once
// Compatibility shim for building this library on macOS clang + libc++.
// libstdc++ exposes __lg in <bits/stl_algobase.h>; libc++ does not.
// CI uses GCC/libstdc++, so this header is intentionally a no-op there.
//
// Intended usage: add `-include clang_compat.hpp` to CXXFLAGS on Darwin.

#if defined(__clang__) && !defined(__GLIBCXX__)

inline constexpr int __lg(int n) noexcept {
 return 31 - __builtin_clz(static_cast<unsigned>(n));
}
inline constexpr int __lg(unsigned n) noexcept {
 return 31 - __builtin_clz(n);
}
inline constexpr int __lg(long n) noexcept {
 return sizeof(long) == 8
  ? 63 - __builtin_clzll(static_cast<unsigned long long>(n))
  : 31 - __builtin_clz(static_cast<unsigned>(n));
}
inline constexpr int __lg(unsigned long n) noexcept {
 return sizeof(long) == 8
  ? 63 - __builtin_clzll(n)
  : 31 - __builtin_clz(static_cast<unsigned>(n));
}
inline constexpr int __lg(long long n) noexcept {
 return 63 - __builtin_clzll(static_cast<unsigned long long>(n));
}
inline constexpr int __lg(unsigned long long n) noexcept {
 return 63 - __builtin_clzll(n);
}

#endif
