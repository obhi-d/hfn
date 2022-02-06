#pragma once

#include "../config.hpp"

#if defined(__clang__)
#define CPPHASH_FUNC_NAME  __PRETTY_FUNCTION__
#define CPPHASH_FUNC_START sizeof("auto hfn::detail::type_name() [T = ") - 1
#define CPPHASH_FUNC_END   sizeof("]") - 1
#elif defined(__GNUC__) || defined(__GNUG__)
#define CPPHASH_FUNC_NAME  __PRETTY_FUNCTION__
#define CPPHASH_FUNC_START sizeof("constexpr auto hfn::detail::type_name() [with T = ") - 1
#define CPPHASH_FUNC_END   sizeof("]") - 1
#elif defined(_MSC_VER)
#define CPPHASH_FUNC_NAME  __FUNCSIG__
#define CPPHASH_FUNC_START sizeof("auto __cdecl hfn::detail::test_name<") - 1
#define CPPHASH_FUNC_END   sizeof(">(void)") - 1
#endif
