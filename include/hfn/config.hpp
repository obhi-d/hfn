#pragma once

#if defined(_MSC_VER)
#define HFN_FORCE_INLINE __forceinline
#include <stdlib.h>
// Other compilers
#else // defined(_MSC_VER)
#define HFN_FORCE_INLINE inline __attribute__((always_inline))
#endif // !defined(_MSC_VER)

#include <cstdint>
#include <tuple>

namespace hfn
{
using uhash128_t = std::tuple<std::uint64_t, std::uint64_t>;

}
