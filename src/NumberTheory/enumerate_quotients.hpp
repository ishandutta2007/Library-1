#pragma once
#include <vector>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <cstdint>
// (q,l,r) : i in (l,r], ⌊N/i⌋ = q
std::vector<std::tuple<uint64_t, uint64_t, uint64_t>> enumerate_quotients(uint64_t N) {
 uint64_t sq= std::sqrt(N), prev= N, x;
 std::vector<std::tuple<uint64_t, uint64_t, uint64_t>> ret;
 for (int q= 1, n= (sq * sq + sq <= N ? sq : sq - 1); q <= n; ++q) ret.emplace_back(q, x= double(N) / (q + 1), prev), prev= x;
 for (int l= sq; l >= 1; --l) ret.emplace_back(double(N) / l, l - 1, l);
 return ret;
}