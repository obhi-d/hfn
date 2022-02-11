
#include "hfn/detail/murmur3.hpp"
#include "hfn/murmur3.hpp"

namespace hfn::detail
{

uhash128_t murmur3_update(uhash128_t prev, void const* key, std::size_t len)
{
  const std::uint8_t* data    = (const std::uint8_t*)key;
  const long          nblocks = static_cast<long>(len / 16);
  const std::size_t   len4    = static_cast<std::size_t>(nblocks) * 16;

  std::uint64_t h1 = std::get<0>(prev);
  std::uint64_t h2 = std::get<1>(prev);

  constexpr std::uint64_t c1 = BIG_CONSTANT(0x87c37b91114253d5);
  constexpr std::uint64_t c2 = BIG_CONSTANT(0x4cf5ad432745937f);

  //----------
  // body

  const std::uint64_t* blocks = (const std::uint64_t*)(data);

  for (long i = 0; i < nblocks; i++)
  {
    std::uint64_t k1 = detail::getblock64(blocks, i * 2 + 0);
    std::uint64_t k2 = detail::getblock64(blocks, i * 2 + 1);

    k1 *= c1;
    k1 = ROTL64(k1, 31);
    k1 *= c2;
    h1 ^= k1;

    h1 = ROTL64(h1, 27);
    h1 += h2;
    h1 = h1 * 5 + 0x52dce729;

    k2 *= c2;
    k2 = ROTL64(k2, 33);
    k2 *= c1;
    h2 ^= k2;

    h2 = ROTL64(h2, 31);
    h2 += h1;
    h2 = h2 * 5 + 0x38495ab5;
  }

  //----------
  // tail

  const std::uint8_t* tail = (const std::uint8_t*)(data + len4);

  std::uint64_t k1 = 0;
  std::uint64_t k2 = 0;

  switch (len & 15)
  {
  case 15:
    k2 ^= std::uint64_t(tail[14]) << 48;
  case 14:
    k2 ^= std::uint64_t(tail[13]) << 40;
  case 13:
    k2 ^= std::uint64_t(tail[12]) << 32;
  case 12:
    k2 ^= std::uint64_t(tail[11]) << 24;
  case 11:
    k2 ^= std::uint64_t(tail[10]) << 16;
  case 10:
    k2 ^= std::uint64_t(tail[9]) << 8;
  case 9:
    k2 ^= std::uint64_t(tail[8]) << 0;
    k2 *= c2;
    k2 = ROTL64(k2, 33);
    k2 *= c1;
    h2 ^= k2;

  case 8:
    k1 ^= std::uint64_t(tail[7]) << 56;
  case 7:
    k1 ^= std::uint64_t(tail[6]) << 48;
  case 6:
    k1 ^= std::uint64_t(tail[5]) << 40;
  case 5:
    k1 ^= std::uint64_t(tail[4]) << 32;
  case 4:
    k1 ^= std::uint64_t(tail[3]) << 24;
  case 3:
    k1 ^= std::uint64_t(tail[2]) << 16;
  case 2:
    k1 ^= std::uint64_t(tail[1]) << 8;
  case 1:
    k1 ^= std::uint64_t(tail[0]) << 0;
    k1 *= c1;
    k1 = ROTL64(k1, 31);
    k1 *= c2;
    h1 ^= k1;
  }

  return uhash128_t(h1, h2);
}

std::uint32_t murmur3_update(std::uint32_t h1, void const* key, std::size_t len)
{
  const uint8_t*     data    = (const uint8_t*)key;
  const std::int32_t nblocks = static_cast<std::int32_t>(len / 4);
  const std::size_t  len4    = static_cast<std::size_t>(nblocks) * 4;

  constexpr std::uint32_t c1 = 0xcc9e2d51;
  constexpr std::uint32_t c2 = 0x1b873593;

  //----------
  // body

  const std::uint32_t* blocks = (const std::uint32_t*)(data + len4);

  for (std::int32_t i = -nblocks; i; i++)
  {
    std::uint32_t k1 = detail::getblock32(blocks, i);

    k1 *= c1;
    k1 = ROTL32(k1, 15);
    k1 *= c2;

    h1 ^= k1;
    h1 = ROTL32(h1, 13);
    h1 = h1 * 5 + 0xe6546b64;
  }

  //----------
  // tail

  const uint8_t* tail = (const uint8_t*)(data + len4);

  std::uint32_t k1 = 0;

  switch (len & 3)
  {
  case 3:
    k1 ^= tail[2] << 16;
  case 2:
    k1 ^= tail[1] << 8;
  case 1:
    k1 ^= tail[0];
    k1 *= c1;
    k1 = ROTL32(k1, 15);
    k1 *= c2;
    h1 ^= k1;
  };

  return h1;
}

} // namespace hfn::detail