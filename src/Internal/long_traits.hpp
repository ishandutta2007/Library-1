#pragma once
#include <cstdint>
// clang-format off
template<class T>struct make_long{using type= T;};
template<>struct make_long<int8_t>{using type= int16_t;};
template<>struct make_long<uint8_t>{using type= uint16_t;};
template<>struct make_long<int16_t>{using type= int32_t;};
template<>struct make_long<uint16_t>{using type= uint32_t;};
template<>struct make_long<int32_t>{using type= int64_t;};
template<>struct make_long<uint32_t>{using type= uint64_t;};
template<>struct make_long<int64_t>{using type= __int128_t;};
template<>struct make_long<uint64_t>{using type= __uint128_t;};
template<>struct make_long<float>{using type= double;};
template<>struct make_long<double>{using type= long double;};
template<class T> using make_long_t= typename make_long<T>::type;
// clang-format on