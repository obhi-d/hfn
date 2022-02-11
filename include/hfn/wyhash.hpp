#pragma once
#include "config.hpp"
#include "wyhash.h"
#include "wyhash32.h"

//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

// Note - The x86 and x64 versions do _not_ produce the same results, as the
// algorithms are optimized for their respective platforms. You can still
// compile and run any of them on any platform, but your performance with the
// non-native version will be less than optimal.
//-----------------------------------------------------------------------------

namespace hfn::wyhash
{

template <typename base>
class hash;

template <>
class hash<std::uint32_t>
{
public:
  HFN_FORCE_INLINE constexpr hash() noexcept = default;
  HFN_FORCE_INLINE constexpr hash(std::uint32_t initial) noexcept : value(initial) {}

  HFN_FORCE_INLINE constexpr auto operator()() const noexcept
  {
    return value;
  }

  HFN_FORCE_INLINE auto operator()(void const* key, std::size_t len) noexcept
  {
    return (value = ::wyhash32(key, len, value));
  }

  template <typename T>
  HFN_FORCE_INLINE auto operator()(T const& key) noexcept
  {
    return (*this)(&key, sizeof(T));
  }

  inline auto operator<=>(hash const&) const noexcept = default;

private:
  std::uint32_t value = 0xaaaaaaaa;
};

template <>
class hash<std::uint64_t>
{
public:
  HFN_FORCE_INLINE hash() noexcept
  {
    make_secret(value, secret);
  }
  HFN_FORCE_INLINE hash(std::uint64_t initial) noexcept : value(initial)
  {
    make_secret(value, secret);
  }

  HFN_FORCE_INLINE auto operator()() const noexcept
  {
    return value;
  }

  HFN_FORCE_INLINE auto operator()(void const* key, std::size_t len) noexcept
  {
    return (value = ::wyhash(key, len, value, secret));
  }

  template <typename T>
  HFN_FORCE_INLINE auto operator()(T const& key) noexcept
  {
    return (*this)(&key, sizeof(T));
  }

  inline auto operator<=>(hash const&) const noexcept = default;

private:
  std::uint64_t value = 0xaaaaaaaa;
  std::uint64_t secret[4];
};

inline std::uint64_t compute64(void const* key, std::size_t len)
{
  hash<std::uint64_t> h;
  h(key, len);
  return h();
}

inline std::uint32_t compute32(void const* key, std::size_t len)
{
  hash<std::uint32_t> h;
  h(key, len);
  return h();
}

template <typename base>
inline base compute(void const* source, std::size_t len)
{
  hash<base> h;
  h(source, len);
  return h();
}

template <typename base, typename T>
inline auto compute(T const& mem)
{
  return compute<base>(&mem, sizeof(T));
}

} // namespace hfn::wyhash

namespace hfn
{
using wyhash_32 = hfn::wyhash::hash<std::uint32_t>;
using wyhash_64 = hfn::wyhash::hash<std::uint64_t>;
} // namespace hfn
