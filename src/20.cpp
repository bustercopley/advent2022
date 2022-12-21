#include "precompiled.hpp"

void solve(std::istream &stream) {
  std::int64_t results[2]{};
  std::vector<std::int64_t> lists[2];
  lists[0].reserve(5000);
  for (std::string line; std::getline(stream, line);) {
    lists[0].push_back(string_to<std::int64_t>(line));
  }
  const int n = (int)std::size(lists[0]);
  lists[1] = lists[0];
  for (auto &element: lists[1]) { element *= 811589153ll; }

  for (int is_part_two = 0; is_part_two != 2; ++is_part_two) {
    const auto &l = lists[is_part_two];
    // Initialize p to the identity permutation
    std::vector<int> p(n);
    for (int i{}; i != n; ++i) { p[i] = i; }

    for (int repeats = 0; repeats != (is_part_two ? 10 : 1); ++repeats) {
      for (int i{}; i != n; ++i) {
        // Find the number of positions to move by
        // Moving an element n - 1 positions has no effect
        if (int m = ((int)(l[i] % (n - 1)) + (n - 1)) % (n - 1)) {
          // Locate the element to move and the position to move it to
          int j = std::ranges::find(p, i) - std::begin(p);
          int k = (j + m) % n;
          // That's a rotate
          if (k > j) {
            std::rotate(&p[j], &p[j + 1], &p[k + 1]);
          } else {
            std::rotate(&p[k + 1], &p[j], &p[j + 1]);
          }
        }
      }
    }

    int i = std::ranges::find(l, 0) - std::begin(l);
    int j = std::ranges::find(p, i) - std::begin(p);
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
