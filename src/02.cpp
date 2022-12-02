#include "precompiled.hpp"

auto regex = re::regex(R"(([ABC]) ([XYZ]))");

void solve(std::istream &stream) {
  std::uint64_t result1{};
  std::uint64_t result2{};

  for (std::string line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      auto p1 = re::match_view(m, 1, line)[0] - 'A';
      auto p2 = re::match_view(m, 2, line)[0] - 'X';
      result1 += 1 + p2 + ((4 + p2 - p1) % 3) * 3;
      result2 += 1 + (2 + p1 + p2) % 3 + p2 * 3;
    }
  }

  std::printf("Part 1 result %llu\n"
              "Part 2 result %llu\n",
    result1, result2);
}

int main() {
  constexpr int day = 2;
  std::printf("Day %d\n", day);
  for (auto pattern: {"test/%02d", "input/%02d"}) {
    auto filename = string_printf(pattern, day);
    if (std::ifstream stream(std::data(filename)); stream) {
      std::printf("\nReading file %s\n", std::data(filename));
      solve(stream);
    }
  }

  return 0;
}
