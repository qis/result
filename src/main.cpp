#include <result.hpp>
#include <fmt/format.h>
#include <string_view>
#include <cstdio>

result<void> simple() noexcept {
  return {};
}

result<std::string_view> copmlex() noexcept {
  co_await simple();
  co_return "test";
}

result<int> coroutine() noexcept {
  const auto s = co_await copmlex();
  co_return static_cast<int>(s.size());
}

int main() {
  if (const auto result = coroutine()) {
    fmt::print("value: {}\n", result.value());
  } else {
    fmt::print("error: {}\n", result.error());
  }
}
