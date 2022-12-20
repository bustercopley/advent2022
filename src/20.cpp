#include "precompiled.hpp"

auto regex = re::regex(R"((-?\d+))");

void solve(std::istream &stream) {
  std::int64_t results[2]{};
  std::vector<std::int64_t> l;
  l.reserve(5000);
  for (std::string line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      l.push_back(string_to<std::int64_t>(match_view(m, 1, line)));
    }
  }
  const int n = (int)std::size(l);
  auto saved_copy = l;

  for (int is_part_two = 0; is_part_two != 2; ++is_part_two) {
    if (is_part_two) {
      l = std::move(saved_copy);
      for (auto &element: l) { element *= 811589153ll; }
    }

    // Initialize p to the identity permutation
    std::vector<int> p(n);
    for (int i{}; i != n; ++i) { p[i] = i; }

    for (int repeats = 0; repeats != (is_part_two ? 10 : 1); ++repeats) {
      for (int i{}; i != n; ++i) {
        // Locate the element to move
        int j{};
        while (p[j] != i) ++j;
        // Find the direction and magnitude of the movement
        // Moving an element n - 1 positions has no effect
        int d = (l[i] > 0) - (l[i] < 0);
        int m = (int)(std::abs(l[i]) % (n - 1));
        // Shift m elements backwards one position
        for (std::int64_t k{0}; k != m; ++k) {
          int r = (j + n + d) % n;
          p[j] = p[r];
          j = r;
        }
        // Move the element into the gap
        p[j] = i;
      }
    }

    int j{};
    while (l[p[j]]) ++j;
    results[is_part_two] =
      l[p[(j + 1000) % n]] + l[p[(j + 2000) % n]] + l[p[(j + 3000) % n]];
  }

  std::printf("Part 1 result %lld\n"
              "Part 2 result %lld\n",
    results[0], results[1]);
}

int main() {
  constexpr int day = 20;
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
