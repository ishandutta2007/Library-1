#pragma once
#include <type_traits>
#define _DETECT_BOOL(name, ...) \
 template <class, class= void> struct name: std::false_type {}; \
 template <class T> struct name<T, std::void_t<__VA_ARGS__>>: std::true_type {}; \
 template <class T> static constexpr bool name##_v= name<T>::value
#define _DETECT_TYPE(name, type1, type2, ...) \
 template <class T, class= void> struct name { \
  using type= type2; \
 }; \
 template <class T> struct name<T, std::void_t<__VA_ARGS__>> { \
  using type= type1; \
 }
