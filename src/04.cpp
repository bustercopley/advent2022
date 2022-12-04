#include "precompiled.hpp"

auto regex = re::regex(R"(^(\d+)-(\d+),(\d+)-(\d+)$)");

void solve(std::istream &stream) {
  std::uint64_t result1{};
  std::uint64_t result2{};

  for (std::string line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      auto a = string_to<int>(match_view(m, 1, line));
      auto b = string_to<int>(match_view(m, 2, line));
      auto c = string_to<int>(match_view(m, 3, line));
      auto d = string_to<int>(match_view(m, 4, line));
      result1 += ((a <= c && d <= b) || (c <= a && b <= d));
      result2 += !(b < c || d < a);
    }
  }

  std::printf("Part 1 result %llu\n"
              "Part 2 result %llu\n",
    result1, result2);
}

int main() {
  constexpr int day = 4;
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
