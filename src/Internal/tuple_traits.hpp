#pragma once
#include <tuple>
#include <array>
#include <type_traits>
#include <cstddef>
template <class T> static constexpr bool tuple_like_v= false;
template <class... Args> static constexpr bool tuple_like_v<std::tuple<Args...>> = true;
template <class T, class U> static constexpr bool tuple_like_v<std::pair<T, U>> = true;
template <class T, size_t K> static constexpr bool tuple_like_v<std::array<T, K>> = true;
template <class T> auto to_tuple(const T &t) {
 if constexpr (tuple_like_v<T>) return std::apply([](auto &&...x) { return std::make_tuple(x...); }, t);
}
template <class T> auto forward_tuple(const T &t) {
 if constexpr (tuple_like_v<T>) return std::apply([](auto &&...x) { return std::forward_as_tuple(x...); }, t);
}
template <class T> static constexpr bool array_like_v= false;
template <class T, size_t K> static constexpr bool array_like_v<std::array<T, K>> = true;
template <class T, class U> static constexpr bool array_like_v<std::pair<T, U>> = std::is_convertible_v<T, U>;
template <class T> static constexpr bool array_like_v<std::tuple<T>> = true;
template <class T, class U, class... Args> static constexpr bool array_like_v<std::tuple<T, U, Args...>> = array_like_v<std::tuple<T, Args...>> && std::is_convertible_v<U, T>;
template <class T> auto to_array(const T &t) {
 if constexpr (array_like_v<T>) return std::apply([](auto &&...x) { return std::array{x...}; }, t);
}
template <class T> using to_tuple_t= decltype(to_tuple(T()));
template <class T> using to_array_t= decltype(to_array(T()));
template <class T> struct other_than_first_argument_type_impl {
 using type= void;
};
template <class T, class... Args> struct other_than_first_argument_type_impl<std::tuple<T, Args...>> {
 using type= std::tuple<Args...>;
};
template <class T> using other_than_first_argument_type_t= typename other_than_first_argument_type_impl<T>::type;