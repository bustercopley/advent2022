#include "precompiled.hpp"
#include <cmath>

auto regex = re::regex(R"((\d+)?)");

void solve(std::istream &stream) {
  std::uint64_t max[4]{};
  std::uint64_t total{};
  for (std::string line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        total += string_to<std::uint64_t>(match_string(m, 1));
      } else {
        max[3] = total;
        std::ranges::sort(max, std::greater<>{});
        total = 0;
      }
    }
  }
  max[3] = total;
  std::ranges::sort(max, std::greater<>{});

  std::printf("Part 1 result %llu\n"
              "Part 2 result %llu\n",
    max[0], max[0] + max[1] + max[2]);
}

int main() {
  constexpr int day = 1;
  std::printf("Day %d\n", day);
  for (auto pattern: {"test/%02d", "input/%02d"}) {
    auto filename = string_printf(pattern, day);
    if (std::ifstream stream(std::data(filename)); stream) {
      solve(stream);
    }
  }

  return 0;
}
