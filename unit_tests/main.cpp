#define CATCH_CONFIG_MAIN
#include "hfn/config.hpp"
#include "hfn/cxstring.hpp"
#include "hfn/digest.hpp"
#include "hfn/fnv1a.hpp"
#include "hfn/murmur3.hpp"
#include "hfn/rhid.hpp"
#include "hfn/type_hash.hpp"
#include "hfn/wyhash.hpp"
#include "hfn/xxhash.hpp"
#include <catch2/catch.hpp>
#include <iostream>

class type_name_test
{};

TEST_CASE("Asserts", "[asserts]")
{
  static_assert(sizeof(hfn::uhash128_t) == 16, "Size is incorrect");
  CHECK(hfn::digest<std::uint64_t>(0) == "0");
  CHECK(hfn::type_name<type_name_test>() == "class type_name_test");
  constexpr std::uint32_t cth = hfn::type_hash<std::string_view>();
  static_assert(cth == 787592100);
  CHECK(hfn::digest<std::uint64_t>(0xff64ffaadd41) == "1anlqvjcv7");
}

TEST_CASE("Murmur3", "[murmur]")
{
  std::string text = "The director instructed us to do batshit crazy things in the past.";

  std::string first    = "First string.";
  std::string second   = "Second string.";
  std::string combined = text + first + second;

  hfn::murmur3_32 hash32;
  hash32(text.c_str(), text.length());
  CHECK(hash32() == hfn::murmur3::compute32(text.c_str(), text.length()));

  hfn::murmur3_128 hash128;
  hash128(text.c_str(), text.length());
  CHECK(hash128() == hfn::murmur3::compute128(text.c_str(), text.length()));

  std::string_view conflict1 = "vertexColor";
  std::string_view conflict2 = "userDefined";
  CHECK(hfn::murmur3::compute32(conflict2.data(), conflict2.length()) !=
        hfn::murmur3::compute32(conflict1.data(), conflict1.length()));

  hfn::cxstring s = "fixdStrinTest";
  CHECK(s.hash() == hfn::murmur3::compute32(s.data(), s.size()));
}

TEST_CASE("Fnv1a", "[fnv1a]")
{
  std::string text = "The director instructed us to do batshit crazy things in the past.";

  std::string first    = "First string.";
  std::string second   = "Second string.";
  std::string combined = text + first + second;

  hfn::fnv1a_32 hash32;
  hash32(text.c_str(), text.length());
  CHECK(hash32() == hfn::fnv1a::compute32(text.c_str(), text.length()));

  hfn::fnv1a_64 hash64;
  hash64(text.c_str(), text.length());
  CHECK(hash64() == hfn::fnv1a::compute64(text.c_str(), text.length()));

  hash32(first.c_str(), first.length());
  hash32(second.c_str(), second.length());
  CHECK(hash32() == hfn::fnv1a::compute32(combined.c_str(), combined.length()));

  hash64(first.c_str(), first.length());
  hash64(second.c_str(), second.length());
  CHECK(hash64() == hfn::fnv1a::compute64(combined.c_str(), combined.length()));
}

TEST_CASE("Wyhash", "[wyhash]")
{
  std::string text = "The director instructed us to do batshit crazy things in the past.";

  hfn::wyhash_32 hash32;
  hash32(text.c_str(), text.length());
  CHECK(hash32() == hfn::wyhash::compute32(text.c_str(), text.length()));

  hfn::wyhash_64 hash64;
  hash64(text.c_str(), text.length());
  CHECK(hash64() == hfn::wyhash::compute64(text.c_str(), text.length()));

  hash32 = hfn::wyhash_32();
  hash32(text.c_str(), 10);
  hash32(text.c_str() + 10, text.length() - 10);
  // CHECK(hash32() == hfn::wyhash::compute32(text.c_str(), text.length()));

  hash64 = hfn::wyhash_64();
  hash64(text.c_str(), 10);
  hash64(text.c_str() + 10, text.length() - 10);
  // CHECK(hash64() == hfn::wyhash::compute64(text.c_str(), text.length()));
}

TEST_CASE("X3hash", "[xxhash]")
{
  std::string text = "The director instructed us to do batshit crazy things in the past.";

  std::string first    = "First string.";
  std::string second   = "Second string.";
  std::string combined = text + first + second;

  hfn::xxhash_64 hash64;
  hash64(text.c_str(), text.length());
  CHECK(hash64() == hfn::xxhash::compute64(text.c_str(), text.length()));

  hfn::xxhash_128 hash128;
  hash64(text.c_str(), text.length());
  // CHECK(hash128() == hfn::xxhash::compute128(text.c_str(), text.length()));

  hash64(first.c_str(), first.length());
  hash64(second.c_str(), second.length());
  // CHECK(hash64() == hfn::xxhash::compute64(combined.c_str(), combined.length()));

  hash128(first.c_str(), first.length());
  hash128(second.c_str(), second.length());
  // CHECK(hash128() == hfn::xxhash::compute128(combined.c_str(), combined.length()));
}

TEST_CASE("RHID", "[rhid]")
{
  std::string text   = "The director instructed us to do batshit crazy things in the past.";
  std::string first  = "First string.";
  std::string second = "Second string.";

  CHECK(hfn::rhid() == hfn::null_rhid);

  hfn::rhid rhid;
  rhid.update(text.c_str(), text.length());

  CHECK((std::string)rhid == "mkjl131");
}
