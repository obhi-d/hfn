#pragma once
#include "config.hpp"
#include "detail/murmur3.hpp"

//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

// Note - The x86 and x64 versions do _not_ produce the same results, as the
// algorithms are optimized for their respective platforms. You can still
// compile and run any of them on any platform, but your performance with the
// non-native version will be less than optimal.
//-----------------------------------------------------------------------------

namespace hfn::murmur3
{

template <typename base>
class hash
{
public:
  HFN_FORCE_INLINE hash() noexcept : value(detail::murmur3_default_seed<base>::value) {}
  HFN_FORCE_INLINE hash(base initial) noexcept : value(initial) {}

  HFN_FORCE_INLINE base operator()() const noexcept
  {
    return detail::murmur3_finalize(value, length);
  }

  HFN_FORCE_INLINE auto operator()(void const* key, std::size_t len) noexcept
  {
    detail::murmur3_update(value, key, len);
    length += len;
    return (*this)();
  }

  template <typename T>
  HFN_FORCE_INLINE base operator()(T const& key) noexcept
  {
    (*this)(&key, sizeof(T));
    return (*this)();
  }

private:
  base        value;
  std::size_t length = 0;
};

inline uhash128_t compute128(void const* key, std::size_t len) noexcept
{
  uhash128_t value = detail::murmur3_default_seed<uhash128_t>::value;
  detail::murmur3_update(value, key, len);
  return detail::murmur3_finalize(value, len);
}

inline std::uint32_t compute32(void const* key, std::size_t len) noexcept
{
  std::uint32_t value = detail::murmur3_default_seed<std::uint32_t>::value;
  detail::murmur3_update(value, key, len);
  return detail::murmur3_finalize(value, len);
}

template <typename base>
inline base compute(void const* source, std::size_t len) noexcept
{
  hash<base> h;
  h(source, len);
  return h();
}

template <typename base, typename T>
inline auto compute(T const& mem) noexcept
{
  return compute<base>(&mem, sizeof(T));
}

} // namespace hfn::murmur3

namespace hfn
{
using murmur3_32  = hfn::murmur3::hash<std::uint32_t>;
using murmur3_128 = hfn::murmur3::hash<hfn::uhash128_t>;
} // namespace hfn
