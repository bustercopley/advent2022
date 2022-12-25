#include "precompiled.hpp"
#include <ranges>

char digits[5] = {'=', '-', '0', '1', '2'};

std::int64_t from_snafu(const std::string &s) {
  std::int64_t result{};
  for (auto d: s) {
    result *= 5;
    result += (std::ranges::find(digits, d) - std::begin(digits)) - 2;
  }
  return result;
}

std::string to_snafu(std::int64_t n) {
  std::string result{};
  while (n) {
    for (int d = 0; d != 5; ++d) {
      if ((n - (d - 2)) % 5 == 0){
        result += digits[d];
        n -= (d - 2);
        n /= 5;
        break;
      }
    }
  }
  std::ranges::reverse(result);
  return result;
}

void solve(std::istream &stream) {
  std::int64_t result{};
  for (std::string line; std::getline(stream, line);) {
    result += from_snafu(line);
  }
  std::printf("Part 1 result\n%s\n", std::data(to_snafu(result)));
}


int main() {
  constexpr int day = 25;
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
