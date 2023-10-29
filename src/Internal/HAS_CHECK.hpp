#pragma once
#include <type_traits>
#define MEMBER_MACRO(member, Dummy, name, type1, type2, last) \
 template <class tClass> struct name##member { \
  template <class U, Dummy> static type1 check(U *); \
  static type2 check(...); \
  static tClass *mClass; \
  last; \
 };
#define HAS_CHECK(member, Dummy) MEMBER_MACRO(member, Dummy, has_, std::true_type, std::false_type, static const bool value= decltype(check(mClass))::value)
#define HAS_MEMBER(member) HAS_CHECK(member, int dummy= (&U::member, 0))
#define HAS_TYPE(member) HAS_CHECK(member, class dummy= typename U::member)
#define HOGE_OR(member, name, type2) \
 MEMBER_MACRO(member, class dummy= typename U::member, name, typename U::member, type2, using type= decltype(check(mClass))) \
 template <class tClass> using name##member##_t= typename name##member<tClass>::type;
#define NULLPTR_OR(member) HOGE_OR(member, nullptr_or_, std::nullptr_t);
#define MYSELF_OR(member) HOGE_OR(member, myself_or_, tClass);