#pragma once
#include "config.hpp"
#include "xxhash.h"

namespace hfn::xxhash
{

template <typename base>
class hash;

template <>
class hash<uhash128_t>
{
public:
  HFN_FORCE_INLINE hash() noexcept
  {
    state = XXH3_createState();
    XXH3_128bits_reset_withSeed(state, 0);
  }

  HFN_FORCE_INLINE hash(std::uint64_t initial) noexcept
  {
    state = XXH3_createState();
    XXH3_128bits_reset_withSeed(state, initial);
  }

  ~hash()
  {
    XXH3_freeState(state);
  }

  HFN_FORCE_INLINE auto operator()() const noexcept
  {
    auto r = XXH3_128bits_digest(state);
    return uhash128_t(r.low64, r.high64);
  }

  HFN_FORCE_INLINE auto operator()(void const* key, std::size_t len) noexcept
  {
    XXH3_128bits_update(state, key, len);
  }

  template <typename T>
  HFN_FORCE_INLINE void operator()(T const& key) noexcept
  {
    (*this)(&key, sizeof(T));
  }

private:
  XXH3_state_t* state = nullptr;
};

template <>
class hash<std::uint64_t>
{
public:
  HFN_FORCE_INLINE hash() noexcept
  {
    state = XXH3_createState();
    XXH3_64bits_reset_withSeed(state, 0);
  }

  HFN_FORCE_INLINE hash(std::uint64_t initial) noexcept
  {
    state = XXH3_createState();
    XXH3_64bits_reset_withSeed(state, initial);
  }

  ~hash()
  {
    XXH3_freeState(state);
  }

  HFN_FORCE_INLINE auto operator()() const noexcept
  {
    return XXH3_64bits_digest(state);
  }

  HFN_FORCE_INLINE auto operator()(void const* key, std::size_t len) noexcept
  {
    XXH3_64bits_update(state, key, len);
  }

  template <typename T>
  HFN_FORCE_INLINE void operator()(T const& key) noexcept
  {
    (*this)(&key, sizeof(T));
  }

private:
  XXH3_state_t* state = nullptr;
};

std::uint64_t compute64(void const* key, std::size_t len)
{
  return XXH3_64bits(key, len);
}

auto compute128(void const* key, std::size_t len)
{
  auto r = XXH3_128bits(key, len);
  return uhash128_t(r.low64, r.high64);
}

template <typename base>
inline base compute(void const* source, std::size_t len)
{
  if constexpr (sizeof(base) == 8)
    return compute64(source, len);
  else
    return compute128(source, len);
}

template <typename base, typename T>
inline auto compute(T const& mem)
{
  return compute<base>(&mem, sizeof(T));
}

} // namespace hfn::xxhash

namespace hfn
{
using xxhash_64  = hfn::xxhash::hash<std::uint64_t>;
using xxhash_128 = hfn::xxhash::hash<uhash128_t>;
} // namespace hfn
