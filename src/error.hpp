#pragma once
#include <fmt/format.h>
#include <limits>
#include <string_view>

enum class error : int {
  success = 0,
  failure = 1,
  invalid = std::numeric_limits<int>::max(),
};

constexpr inline std::string_view to_string(error error) noexcept {
  switch (error) {
  case error::success:
    return "success";
  case error::failure:
    return "failure";
  case error::invalid:
    return "invalid";
  }
  return "unknown";
}

namespace fmt {

template <>
struct formatter<error> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(error error, FormatContext& ctx) {
    return format_to(ctx.out(), "{}", to_string(error));
  }
};

}  // namespace fmt
