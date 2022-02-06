#include "digest.hpp"
#include "wyhash.hpp"

namespace hfn
{

class rhid
{
public:
  inline constexpr rhid() noexcept            = default;
  inline constexpr rhid(rhid const&) noexcept = default;
  inline constexpr rhid(rhid&&) noexcept      = default;
  inline constexpr rhid& operator=(rhid const&) noexcept = default;
  inline constexpr rhid& operator=(rhid&&) noexcept = default;
  inline constexpr rhid(std::uint32_t val) noexcept : value(val) {}

  template <typename T>
  inline void update(T const& key) noexcept
  {
    value(key);
  }

  inline void update(void const* key, std::size_t len) noexcept
  {
    value(key, len);
  }

  inline operator std::string() const noexcept
  {
    return digest(value());
  }

  inline auto operator<=>(rhid const&) const noexcept = default;

private:
  wyhash_32 value = 0;
};

static constexpr rhid null_rhid = {};

} // namespace hfn