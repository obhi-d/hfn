#pragma once
#include <array>
#include <string>

namespace hfn
{
// clang-format off
static inline constexpr std::array<char, 32> digest_transform = {'0', '1', '2', '3', '4', '5', '6', '7', 
                                                                 '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 
                                                                 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 
                                                                 'o', 'p', 'q', 'r', 's', 't', 'u', 'v'};
// clang-format on

/// @brief Generate an alpha numeric digest from 64bit value, does not include padding, cannot be decoded
template <typename uint_type>
std::string digest(uint_type value) noexcept
{
  if (!value)
    return "0";
  std::string    ret;
  constexpr auto mask = digest_transform.size() - 1;
  while (value)
  {
    ret += digest_transform[(value & mask)];
    value >>= 5;
  }
  return ret;
}

} // namespace hfn