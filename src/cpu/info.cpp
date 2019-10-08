#include <cpu/info.hpp>

#if defined(_WIN32)
#include <intrin.h>
#include <string.h>
#elif defined(__linux__)
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <regex>
#else
#error Unsupported OS.
#endif

namespace cpu {

std::string info() {
  std::string info;
#if defined(_WIN32)
  int id[4] = { -1 };
  __cpuid(id, 0x80000000);
  for (unsigned i = 0x80000002, max = static_cast<unsigned>(id[0]); i < max && i < 0x80000005; i++) {
    __cpuid(id, i);
    auto data = reinterpret_cast<const char*>(id);
    auto size = strnlen_s(data, sizeof(info));
    for (size_t i = 0; i < size; i++) {
      if (data[i] < ' ' || data[i] > '~') {
        size = i;
        break;
      }
    }
    info.append(data, size);
  }
#elif defined(__linux__)
  std::ifstream is("/proc/cpuinfo", std::ios::binary);
  for (std::string line; std::getline(is, line);) {
    if (line.starts_with("model name")) {
      info = std::regex_replace(line, std::regex(R"(model name\s*:\s*)"), "");
      break;
    }
  }
#else
#error Unsupported OS.
#endif
  return info;
}

}  // namespace cpu
