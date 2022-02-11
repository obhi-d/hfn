#pragma once
#include "type_name_ex.hpp"

namespace hfn::detail
{
//-----------------------------------------------------------------------------
// Platform-specific functions and macros

// Microsoft Visual Studio

#if defined(_MSC_VER)

#define ROTL32(x, y)    _rotl(x, y)
#define ROTL64(x, y)    _rotl64(x, y)
#define BIG_CONSTANT(x) (x)

// Other compilers

#else // defined(_MSC_VER)

HFN_FORCE_INLINE std::uint32_t rotl32(std::uint32_t x, int8_t r) noexcept
{
  return (x << r) | (x >> (32 - r));
}
HFN_FORCE_INLINE uint64_t rotl64(uint64_t x, int8_t r) noexcept
{
  return (x << r) | (x >> (64 - r));
}
#define ROTL32(x, y)    rotl32(x, y)
#define ROTL64(x, y)    rotl64(x, y)
#define BIG_CONSTANT(x) (x##LLU)

#endif // !defined(_MSC_VER)

//-----------------------------------------------------------------------------
// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here

HFN_FORCE_INLINE std::uint32_t getblock32(const std::uint32_t* p, int i) noexcept
{
  return p[i];
}
HFN_FORCE_INLINE std::uint64_t getblock64(const std::uint64_t* p, int i) noexcept
{
  return p[i];
}

//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche
HFN_FORCE_INLINE std::uint32_t fmix32(std::uint32_t h) noexcept
{
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}
//----------
HFN_FORCE_INLINE std::uint64_t fmix64(std::uint64_t k) noexcept
{
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xff51afd7ed558ccd);
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
  k ^= k >> 33;

  return k;
}

//-----------------------------------------------------------------------------
HFN_FORCE_INLINE std::uint32_t murmur3_finalize(std::uint32_t value, std::size_t len) noexcept
{
  return fmix32(value ^ static_cast<std::uint32_t>(len));
}

HFN_FORCE_INLINE uhash128_t murmur3_finalize(uhash128_t value, std::size_t len) noexcept
{
  return uhash128_t(fmix64(std::get<0>(value) ^ len), fmix64(std::get<1>(value) ^ len));
}

std::uint32_t murmur3_update(std::uint32_t h1, void const* key, std::size_t len);
uhash128_t    murmur3_update(uhash128_t prev, void const* key, std::size_t len);

template <typename T>
struct murmur3_default_seed;

template <>
struct murmur3_default_seed<std::uint32_t>
{
  static constexpr std::uint32_t value = 0x54F9CA4F;
};

template <>
struct murmur3_default_seed<uhash128_t>
{
  static constexpr uhash128_t value = uhash128_t(0x54F9CA4F, 0x3);
};

} // namespace hfn::detail