#include "precompiled.hpp"

auto regex = re::regex(R"((\d+)?)");

// Mabye insert into sorted container dropping last element
template <typename T> auto insert(auto &top, T &&candidate, auto &&comp) {
  auto n = std::size(top) - 1;
  if (comp(top[n], candidate)) {
    while (n && comp(top[n - 1], candidate)) {
      top[n] = top[n - 1];
      --n;
    }
    top[n] = (T &&) candidate;
  }
}

template <typename T> auto insert(auto &top, T &&candidate) {
  return insert(top, (T &&) candidate, std::less<>{});
}

void solve(std::istream &stream) {
  std::uint64_t top[3]{};
  std::uint64_t calories{};

  for (std::string line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        calories += string_to<std::uint64_t>(match_string(m, 1));
      } else {
        insert(top, calories);
        calories = 0;
      }
    }
  }
  insert(top, calories);

  std::printf("Part 1 result %llu\n"
              "Part 2 result %llu\n",
    top[0], top[0] + top[1] + top[2]);
}

int main() {
  constexpr int day = 1;
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
