#pragma once
#include <type_traits>
#define HAS_CHECK(member, Dummy) \
 template <class tClass> struct has_##member { \
  template <class U, Dummy> static std::true_type check(U *); \
  static std::false_type check(...); \
  static tClass *mClass; \
  static const bool value= decltype(check(mClass))::value; \
 };
#define HAS_MEMBER(member) HAS_CHECK(member, int dummy= (&U::member, 0))
#define HAS_TYPE(member) HAS_CHECK(member, class dummy= typename U::member)