
#pragma once

#include <string_view>
#include <type_traits>

#include "cxstring.hpp"
// Defines required
#include "detail/type_name_ex.hpp"

namespace hfn
{
namespace detail
{
template <typename T>
constexpr auto type_name()
{
  return hfn::cxstring{CPPHASH_FUNC_NAME}.substring(CPPHASH_FUNC_START, CPPHASH_FUNC_END);
}

template <typename T>
constexpr std::uint32_t type_hash()
{
  return type_name<T>().hash();
}

template <typename T>
using cvrt = std::remove_cv_t<std::remove_reference_t<T>>;
} // namespace detail

template <typename T>
constexpr std::string_view type_name()
{
  return std::string_view(detail::type_name<detail::cvrt<T>>().data(), detail::type_name<detail::cvrt<T>>().size());
}

template <typename T>
constexpr std::uint32_t type_hash()
{
  return type_hash<detail::cvrt<T>>();
}
} // namespace hfn