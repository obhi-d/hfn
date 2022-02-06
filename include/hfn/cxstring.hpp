#pragma once

#include <algorithm>
#include <cstdint>
#include <string_view>

namespace hfn
{

class cxstring
{
public:
  template <std::size_t N>
  constexpr cxstring(char const (&a)[N]) noexcept : p(a), sz(N - 1)
  {}
  constexpr cxstring(char const* a, std::size_t const N) noexcept : p(a), sz(N) {}

  constexpr char operator[](std::size_t n) const noexcept
  {
    return n < sz ? p[n] : 0;
  }

  constexpr std::size_t size() const noexcept
  {
    return sz;
  }
  constexpr char const* data() const noexcept
  {
    return p;
  }
  constexpr cxstring substring(const std::size_t start, const std::size_t end) const noexcept
  {
    return cxstring(p + start, size() - (end + start));
  }

  constexpr std::uint32_t hash(const std::uint32_t seed = 0) const noexcept
  {
    return compute_murmur3(seed);
  }

  constexpr std::uint32_t fnv1a_hash() const noexcept
  {
    return compute_fnv1a(p, sz);
  }

  inline constexpr operator std::string_view() const noexcept
  {
    return std::string_view(p, sz);
  }

  inline auto operator<=>(cxstring const&) const noexcept = default;

private:
  static constexpr std::uint32_t compute_fnv1a(char const* const s, std::int32_t count) noexcept
  {
    return ((count ? compute_fnv1a(s, count - 1) : 2166136261u) ^ static_cast<std::uint8_t>(s[count])) * 16777619u;
  }

  constexpr uint32_t get_block(int idx) const noexcept
  {
    int      i  = (static_cast<int>(block_size()) + idx) * 4;
    uint32_t b0 = p[i];
    uint32_t b1 = p[i + 1];
    uint32_t b2 = p[i + 2];
    uint32_t b3 = p[i + 3];
    return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
  }

  constexpr std::size_t block_size() const noexcept
  {
    return sz / 4;
  }

  constexpr char tail(const int n) const noexcept
  {
    int tail_size = sz % 4;
    return p[sz - tail_size + n];
  }

  constexpr std::uint32_t compute_murmur3(const std::uint32_t seed = 0) const noexcept
  {
    std::uint32_t h1 = seed;

    const std::uint32_t c1 = 0xcc9e2d51;
    const std::uint32_t c2 = 0x1b873593;

    const std::int32_t nblocks = static_cast<std::int32_t>(block_size());
    for (std::int32_t i = -nblocks; i; i++)
    {
      std::uint32_t k1 = get_block(i);

      k1 *= c1;
      k1 = (k1 << 15) | (k1 >> (32 - 15));
      k1 *= c2;

      h1 ^= k1;
      h1 = (h1 << 13) | (h1 >> (32 - 13));
      h1 = h1 * 5 + 0xe6546b64;
    }

    std::uint32_t k1 = 0;

    switch (size() & 3)
    {
    case 3:
      k1 ^= tail(2) << 16;
    case 2:
      k1 ^= tail(1) << 8;
    case 1:
      k1 ^= tail(0);
      k1 *= c1;
      k1 = (k1 << 15) | (k1 >> (32 - 15));
      k1 *= c2;
      h1 ^= k1;
    };

    h1 ^= static_cast<std::uint32_t>(size());

    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;
    return h1;
  }

  char const*       p;
  std::size_t const sz;
};

} // namespace hfn