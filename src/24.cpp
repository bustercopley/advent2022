#include "precompiled.hpp"

using point = std::tuple<int, int>;
constexpr point directions[]{{0, -1}, {-1, 0}, {0, 1}, {1, 0}, {0, 0}};
constexpr char sigils[]{'^', '<', 'v', '>', '*'};

auto escape(auto &grid, auto &blizzards, int h, int w, int x) {
  int result1{};
  int result2{};
  int time{1};
  std::set<std::tuple<point, int>> spaces[2]{{{point{x, 0}, 0}}};
  while (true) {
    for (int y = 1; y != h - 1; ++y) {
      for (int x = 1; x != w - 1; ++x) { grid[y][x] = '.'; }
    }
    for (auto &[p, d]: blizzards) {
      auto &[x, y] = p;
      auto [dx, dy] = directions[d];
      x = (x + dx + w - 3) % (w - 2) + 1;
      y = (y + dy + h - 3) % (h - 2) + 1;
      grid[y][x] = '@';
    }
    for (auto [p, score]: spaces[0]) {
      auto [x, y] = p;
      for (int d = 0; d != 5; ++d) {
        auto [dx, dy] = directions[d];
        auto x1 = x + dx;
        auto y1 = y + dy;
        if (x1 >= 0 && x1 < w && y1 >= 0 && y1 < h && grid[y1][x1] == '.') {
          if (((~score & 1) && y1 == h - 1) || ((score & 1) && y1 == 0)) {
            ++score;
          }
          if (score == 1 && !result1) {
            result1 = time;
          }
          if (score == 3) {
            result2 = time;
            return std::make_tuple(result1, result2);
          }
          spaces[1].emplace(point{x1, y1}, score);
        }
      }
    }
    spaces[0] = std::move(spaces[1]);
    ++time;
  }
}

void solve(std::istream &stream) {
  std::vector<std::string> grid;
  for (std::string line; std::getline(stream, line);) {
    grid.push_back(std::move(line));
  }

  int h = std::size(grid);
  int w = std::size(grid[0]);
  int x = std::ranges::find(grid[0], '.') - std::begin(grid[0]);
  std::vector<std::tuple<point, int>> blizzards;
  for (int y{}; const auto &line: grid) {
    for (int x{}; auto c: line) {
      if (c == '^') {
        blizzards.emplace_back(point{x, y}, 0);
      } else if (c == '<') {
        blizzards.emplace_back(point{x, y}, 1);
      } else if (c == 'v') {
        blizzards.emplace_back(point{x, y}, 2);
      } else if (c == '>') {
        blizzards.emplace_back(point{x, y}, 3);
      }
      ++x;
    }
    ++y;
  }
  auto [result1, result2] = escape(grid, blizzards, h, w, x);
  std::printf("Part 1 result %d\n"
              "Part 2 result %d\n",
    result1, result2);
}

int main() {
  constexpr int day = 24;
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
