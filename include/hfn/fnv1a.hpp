#pragma once
#include "config.hpp"
#include "detail/fnv1a.hpp"
#include <type_traits>

namespace hfn::fnv1a
{

template <typename fnvt>
inline auto seed(fnvt initial = 0) noexcept
{
  using type = std::remove_cv_t<std::remove_reference_t<fnvt>>;
  return (initial) ? (detail::fnv1a<sizeof(type)>::offset ^ initial) * detail::fnv1a<sizeof(type)>::prime
                   : detail::fnv1a<sizeof(type)>::offset;
}

template <typename fnvt>
inline auto update(fnvt& last, void const* source, std::size_t len)
{
  const std::uint8_t* values = reinterpret_cast<const std::uint8_t*>(source);
  for (std::size_t i = 0; i < len; ++i)
  {
    last ^= values[i];
    last *= detail::fnv1a<sizeof(fnvt)>::prime;
  }
  return last;
}

template <typename fnvt, typename T>
inline auto update(fnvt& last, T const& mem)
{
  return update(last, &mem, sizeof(mem));
}

template <typename fnvt>
inline fnvt compute(void const* source, std::size_t len)
{
  constexpr fnvt initial = 0;
  auto           result  = seed(initial);
  return update(result, source, len);
}

template <typename fnvt, typename T>
inline auto compute(T const& mem)
{
  return compute<fnvt>(&mem, sizeof(T));
}

inline std::uint64_t compute64(void const* source, std::size_t len)
{
  constexpr std::uint64_t initial = 0;
  auto                    result  = seed(initial);
  return update(result, source, len);
}

template <typename T>
inline auto compute64(T const& mem)
{
  return compute<std::uint64_t>(&mem, sizeof(T));
}

inline std::uint32_t compute32(void const* source, std::size_t len)
{
  constexpr std::uint32_t initial = 0;
  auto                    result  = seed(initial);
  return update(result, source, len);
}

template <typename T>
inline auto compute32(T const& mem)
{
  return compute<std::uint32_t>(&mem, sizeof(T));
}

template <typename base>
class hash
{
public:
  HFN_FORCE_INLINE hash() noexcept : value(seed<base>()) {}
  HFN_FORCE_INLINE hash(base initial) noexcept : value(seed(initial)) {}

  HFN_FORCE_INLINE base operator()() const noexcept
  {
    return value;
  }

  HFN_FORCE_INLINE auto operator()(void const* key, std::size_t len) noexcept
  {
    return update(value, key, len);
  }

  template <typename T>
  HFN_FORCE_INLINE auto operator()(T const& key) noexcept
  {
    return update(value, &key, sizeof(T));
  }

private:
  base value;
};

} // namespace hfn::fnv1a

namespace hfn
{
using fnv1a_32 = hfn::fnv1a::hash<std::uint32_t>;
using fnv1a_64 = hfn::fnv1a::hash<std::uint64_t>;
} // namespace hfn
