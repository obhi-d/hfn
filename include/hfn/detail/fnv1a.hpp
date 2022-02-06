#pragma once
#include <cstdint>

namespace hfn::detail
{
template <const std::uint32_t>
struct fnv1a;
template <>
struct fnv1a<4>
{
  static constexpr std::uint32_t offset = 2166136261u;
  static constexpr std::uint32_t prime  = 16777619u;
};
template <>
struct fnv1a<8>
{
  static constexpr std::uint64_t offset = 14695981039346656037u;
  static constexpr std::uint64_t prime  = 1099511628211u;
};
} // namespace hfn::detail
