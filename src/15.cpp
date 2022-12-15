#include "precompiled.hpp"

auto regex = re::regex(
  R"(^Sensor at x=(-?\d+), y=(-?\d+): closest beacon is at x=(-?\d+), y=(-?\d+)$)");

void solve(std::istream &stream) {

  using region_t = std::tuple<int, int, int, int>;
  std::vector<region_t> regions;
  for (std::string line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      int x0 = string_to<int>(match_view(m, 1, line));
      int y0 = string_to<int>(match_view(m, 2, line));
      int x1 = string_to<int>(match_view(m, 3, line));
      int y1 = string_to<int>(match_view(m, 4, line));
      int d = std::abs(x1 - x0) + std::abs(y1 - y0);
      regions.emplace_back(x0 + y0 - d, x0 + y0 + d, x0 - y0 - d, x0 - y0 + d);
    }
  }

  for (int part_two = 0; part_two != 2; ++part_two) {
    const int first = part_two ? 0 : 2000000;
    const int last = part_two ? 4000000 : 2000000;
    for (int y = first; y != last + 1; ++y) {

      std::vector<std::pair<int, int>> intervals;

      if (part_two) {
        intervals.emplace_back(std::numeric_limits<int>::min(), 0);
        intervals.emplace_back(4000000, std::numeric_limits<int>::max());
      }

      for (auto [dl, ur, ul, dr]: regions) {
        auto l = std::max(dl - y, ul + y);
        auto r = std::min(ur - y, dr + y);
        if (l < r) {
          intervals.emplace_back(l, r);
        }
      loop:
        // Combine intervals
        if (std::size(intervals) >= 2) {
          auto [c, d] = intervals.back();
          for (auto iter = std::begin(intervals);
               iter != std::prev(std::end(intervals)); ++iter) {
            auto [a, b] = *iter;
            if (!(b < c || d < a)) {
              intervals.pop_back();
              std::shift_left(iter, std::end(intervals), 1);
              intervals.back() = std::make_pair(std::min(a, c), std::max(b, d));
              goto loop;
            }
          }
        }
      }

      if (!part_two) {
        int result1{};
        for (auto [a, b]: intervals) { result1 += b - a; }
        std::printf("Part 1 result %d\n", result1);
      } else {
        if (std::size(intervals) == 2) {
          auto [a, b] = intervals[0];
          auto [c, d] = intervals[1];
          auto x = std::min(b, d) + 1;
          auto result2 = 4000000LL * x + y;
          std::printf("Part 2 result %lld\n", result2);
          break;
        }
      }
    }
  }
}

int main() {
  std::printf("Day 15\n");
  for (auto filename: {"input/15"}) {
    if (std::ifstream stream(filename); stream) {
      std::printf("\nReading file %s\n", filename);
      solve(stream);
    }
  }
  return 0;
}
