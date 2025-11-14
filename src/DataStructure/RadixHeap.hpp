#pragma once
#include <bits/stdc++.h>
/**
 * @title Radix-Heap
 * @category データ構造
 * @brief dijkstraの高速化に使用
 * @brief 浮動小数点数も使える
 * @brief 小さい順
 * @see https://github.com/iwiwi/radix-heap/blob/master/README_ja.md
 */

// verify用:
// https://codeforces.com/contest/316/problem/C2
// (Radix-HeapでないとTLEした -> 容量スケーリングならTLEしない)

// BEGIN CUT HERE

namespace internal {
template <bool Is64bit>
class find_bucket_impl;
template <>
class find_bucket_impl<false> {
 public:
  static inline constexpr std::size_t find_bucket(uint32_t x, uint32_t last) {
    return x == last ? 0 : 32 - __builtin_clz(x ^ last);
  }
};
template <>
class find_bucket_impl<true> {
 public:
  static inline constexpr std::size_t find_bucket(std::uint64_t x,
                                                  std::uint64_t last) {
    return x == last ? 0 : 64 - __builtin_clzll(x ^ last);
  }
};
template <typename T>
inline constexpr std::size_t find_bucket(T x, T last) {
  return find_bucket_impl<sizeof(T) == 8>::find_bucket(x, last);
}
template <typename key_t, bool IsSigned>
class encoder_impl_integer;
template <typename key_t>
class encoder_impl_integer<key_t, false> {
 public:
  using ukey_t = key_t;
  inline static constexpr ukey_t encode(key_t x) { return x; }
  inline static constexpr key_t decode(ukey_t x) { return x; }
};
template <typename key_t>
class encoder_impl_integer<key_t, true> {
 public:
  typedef typename std::make_unsigned<key_t>::type ukey_t;
  inline static constexpr ukey_t encode(key_t x) {
    return static_cast<ukey_t>(x)
           ^ (ukey_t(1) << ukey_t(std::numeric_limits<ukey_t>::digits - 1));
  }
  inline static constexpr key_t decode(ukey_t x) {
    return static_cast<key_t>(
        x ^ (ukey_t(1) << (std::numeric_limits<ukey_t>::digits - 1)));
  }
};
template <typename key_t, typename ukey_t>
class encoder_impl_decimal {
 public:
  inline static constexpr ukey_t encode(key_t x) {
    return raw_cast<key_t, ukey_t>(x)
           ^ ((-(raw_cast<key_t, ukey_t>(x)
                 >> (std::numeric_limits<ukey_t>::digits - 1)))
              | (ukey_t(1) << (std::numeric_limits<ukey_t>::digits - 1)));
  }
  inline static constexpr key_t decode(ukey_t x) {
    return raw_cast<ukey_t, key_t>(
        x
        ^ (((x >> (std::numeric_limits<ukey_t>::digits - 1)) - 1)
           | (ukey_t(1) << (std::numeric_limits<ukey_t>::digits - 1))));
  }

 private:
  template <typename T, typename U>
  union raw_cast {
   public:
    constexpr raw_cast(T t) : t_(t) {}
    operator U() const { return u_; }

   private:
    T t_;
    U u_;
  };
};
template <typename key_t>
class encoder
    : public encoder_impl_integer<key_t, std::is_signed<key_t>::value> {};
template <>
class encoder<float> : public encoder_impl_decimal<float, uint32_t> {};
template <>
class encoder<double> : public encoder_impl_decimal<double, std::uint64_t> {};
}  // namespace internal

template <typename key_t, typename val_t,
          typename encoder_t = internal::encoder<key_t>>
class RadixHeap {
 public:
  typedef typename encoder_t::ukey_t ukey_t;

  RadixHeap() : size_(0), last_(), buckets_() {
    buckets_min_.fill(std::numeric_limits<ukey_t>::max());
  }

  void push(key_t key, val_t value) {
    const ukey_t x = encoder_t::encode(key);
    assert(last_ <= x);
    ++size_;
    const std::size_t k = internal::find_bucket(x, last_);
    buckets_[k].emplace_back(x, value);
    buckets_min_[k] = std::min(buckets_min_[k], x);
  }
  void emplace(key_t key, val_t value) { push(key, value); }
  std::pair<key_t, val_t> top() {
    pull();
    return std::make_pair(encoder_t::decode(last_), buckets_[0].back().second);
  }
  std::pair<key_t, val_t> pop() {
    pull();
    --size_;
    auto ret
        = std::make_pair(encoder_t::decode(last_), buckets_[0].back().second);
    buckets_[0].pop_back();
    return ret;
  }
  std::size_t size() const { return size_; }
  bool empty() const { return size_ == 0; }

 private:
  std::size_t size_;
  ukey_t last_;
  std::array<std::vector<std::pair<ukey_t, val_t>>,
             std::numeric_limits<ukey_t>::digits + 1>
      buckets_;
  std::array<ukey_t, std::numeric_limits<ukey_t>::digits + 1> buckets_min_;

  void pull() {
    assert(size_ > 0);
    if (!buckets_[0].empty()) return;
    std::size_t i = 1;
    while (buckets_[i].empty()) ++i;
    last_ = buckets_min_[i];
    for (std::size_t j = 0; j < buckets_[i].size(); ++j) {
      const ukey_t x = buckets_[i][j].first;
      const std::size_t k = internal::find_bucket(x, last_);
      buckets_[k].emplace_back(std::move(buckets_[i][j]));
      buckets_min_[k] = std::min(buckets_min_[k], x);
    }
    buckets_[i].clear();
    buckets_min_[i] = std::numeric_limits<ukey_t>::max();
  }
};
