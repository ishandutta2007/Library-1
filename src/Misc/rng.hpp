#pragma once
#include <random>
#include <cstdint>
uint64_t rng() {
 static uint64_t x= 10150724397891781847ULL * std::random_device{}();
 return x^= x << 7, x^= x >> 9;
}
uint64_t rng(uint64_t lim) { return rng() % lim; }
int64_t rng(int64_t l, int64_t r) { return l + rng() % (r - l); }