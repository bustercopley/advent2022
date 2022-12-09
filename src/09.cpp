#include "precompiled.hpp"

auto regex = re::regex(R"(^[ULDR] (\d+)$)");

void solve(std::istream &stream) {
  std::set<std::tuple<int, int>> visited1{{0, 0}};
  std::set<std::tuple<int, int>> visited9{{0, 0}};
  std::tuple<int, int> knots[10]{};
  for (std::string line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      char direction = line[0];
      int distance = string_to<int>(match_view(m, 1, line));
      for (int i = 0; i != distance; ++i) {
        auto [hx, hy] = knots[0];
        switch (direction) {
        case 'U':
          hy -= 1;
          break;
        case 'L':
          hx -= 1;
          break;
        case 'D':
          hy += 1;
          break;
        default:
          hx += 1;
          break;
        }
        knots[0] = std::make_tuple(hx, hy);
        for (int j = 1; j != 10; ++j) {
          auto [x0, y0] = knots[j - 1];
          auto [x1, y1] = knots[j];
          x1 += (x0 > x1) - (x0 < x1);
          y1 += (y0 > y1) - (y0 < y1);
          if (!(x1 == x0 && y1 == y0)) {
            knots[j] = std::make_tuple(x1, y1);
          }
        }
        visited1.insert(knots[1]);
        visited9.insert(knots[9]);

        // auto minx = std::numeric_limits<int>::max();
        // auto miny = std::numeric_limits<int>::max();
        // auto maxx = std::numeric_limits<int>::min();
        // auto maxy = std::numeric_limits<int>::min();
        // for (auto p: visited1) {
        //   auto [x, y] = p;
        //   minx = std::min(minx, x);
        //   miny = std::min(miny, y);
        //   maxx = std::max(maxx, x);
        //   maxy = std::max(maxy, y);
        // }
        // for (auto p: visited9) {
        //   auto [x, y] = p;
        //   minx = std::min(minx, x);
        //   miny = std::min(miny, y);
        //   maxx = std::max(maxx, x);
        //   maxy = std::max(maxy, y);
        // }
        // for (auto knot: knots) {
        //   auto [x, y] = knot;
        //   minx = std::min(minx, x);
        //   miny = std::min(miny, y);
        //   maxx = std::max(maxx, x);
        //   maxy = std::max(maxy, y);
        // }

        // std::vector<std::string> grid(maxy - miny + 1,
        //   std::string(maxx - minx + 1, '.'));
        // for (auto p: visited1) {
        //   auto [x, y] = p;
        //   grid[y - miny][x - minx] = '#';
        // }
        // for (auto p: visited9) {
        //   auto [x, y] = p;
        //   grid[y - miny][x - minx] = '*';
        // }
        // grid[-miny][-minx] = 's';
        // for (int i = '0'; auto knot: knots) {
        //   auto [x, y] = knot;
        //   grid[y - miny][x - minx] = i;
        //   ++i;
        // }

        // for (const auto &line: grid) { std::printf("%s\n", std::data(line)); }
        // std::printf("\n");
      }
    }
  }

  std::uint64_t result1{std::size(visited1)};
  std::uint64_t result2{std::size(visited9)};
  std::printf("Part 1 result %llu\n"
              "Part 2 result %llu\n",
    result1, result2);
}

int main() {
  constexpr int day = 9;
  std::printf("Day %d\n", day);
  for (auto pattern: {"test/%02d", "test/%02d-2", "input/%02d"}) {
    auto filename = string_printf(pattern, day);
    if (std::ifstream stream(std::data(filename)); stream) {
      std::printf("\nReading file %s\n", std::data(filename));
      solve(stream);
    }
  }

  return 0;
}
