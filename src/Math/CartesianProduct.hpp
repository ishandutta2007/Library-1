#pragma once
#include <tuple>
#include <array>
#include <utility>
template <class... Ks> struct CartesianProduct: std::tuple<Ks...> {
 static constexpr int N= sizeof...(Ks);
 using Self= CartesianProduct;
 using std::tuple<Ks...>::tuple;
 template <class T> CartesianProduct(const T &v) { fill(v, std::make_index_sequence<N>()); }
 template <class T, std::size_t... I> std::array<int, N> fill(const T &v, std::index_sequence<I...>) { return {{(void(std::get<I>(*this)= v), 0)...}}; }
#define HELPER(name, op) \
 template <std::size_t... I> std::array<int, N> name(const Self &y, std::index_sequence<I...>) { return {{(void(std::get<I>(*this) op##= std::get<I>(y)), 0)...}}; } \
 Self &operator op##=(const Self &r) { return name(r, std::make_index_sequence<N>()), *this; }
 HELPER(add_assign, +)
 HELPER(dif_assign, -)
 HELPER(mul_assign, *)
 HELPER(div_assign, /)
#undef HELPER
 Self operator+(const Self &r) const { return Self(*this)+= r; }
 Self operator-(const Self &r) const { return Self(*this)-= r; }
 Self operator*(const Self &r) const { return Self(*this)*= r; }
 Self operator/(const Self &r) const { return Self(*this)/= r; }
};