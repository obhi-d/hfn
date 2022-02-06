#pragma once

#include <algorithm>
#include <cstdint>
#include <string_view>

namespace hfn
{

template <std::size_t N>
struct fixed_string
{
  template <std::size_t N>
  constexpr fixed_string(char const (&a)[N]) : p(a), sz(N - 1)
  {}
  constexpr fixed_string(char const* a, std::size_t const N) : p(a), sz(N) {}

  constexpr char operator[](std::size_t n) const
  {
    return n < sz ? p[n] : 0;
  }

  constexpr std::size_t size() const
  {
    return sz;
  }
  constexpr char const* data() const
  {
    return p;
  }
  constexpr fixed_string substring(const std::size_t start, const std::size_t end) const
  {
    return fixed_string(p + start, size() - (end + start));
  }

  constexpr std::uint32_t hash() const
  {
    return compute(p, static_cast<int>(sz - 1));
  }

  static constexpr std::uint32_t compute(char const* const s, std::int32_t count)
  {
    return ((count ? compute(s, count - 1) : 2166136261u) ^ static_cast<std::uint8_t>(s[count])) * 16777619u;
  }

  inline constexpr operator std::string_view() const
  {
    return std::string_view(p, sz);
  }

  inline auto operator<=>(fixed_string const&) const noexcept = default;

  char const*       p;
  std::size_t const sz;
};

template <std::size_t N>
fixed_string(char const (&a)[N]) -> fixed_string<N>;

} // namespace hfn