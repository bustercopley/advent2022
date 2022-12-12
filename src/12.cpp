#include "precompiled.hpp"

constexpr std::tuple<int, int> directions[]{{0, -1}, {-1, 0}, {0, 1}, {1, 0}};

void solve(std::istream &stream) {
  std::tuple<int, int> start;
  std::tuple<int, int> target;
  std::vector<std::string> grid;
  for (std::string line; std::getline(stream, line);) {
    if (auto pos = line.find('S'); pos != std::string::npos) {
      start = {pos, std::size(grid)};
      line[pos] = 'a';
    }
    if (auto pos = line.find('E'); pos != std::string::npos) {
      target = {pos, std::size(grid)};
      line[pos] = 'z';
    }
    grid.push_back(line);
  }
  const auto height = (int)std::size(grid);
  const auto width = (int)std::size(grid[0]);

  int result1{};
  int result2{};
  int time{};
  std::vector<bool> visited(width * height);
  // Start at the destination and climb down
  std::vector<std::tuple<int, int>> stacks[]{{{target}}, {}};
  {
    auto [x, y] = target;
    visited[y * width + x] = true;
  }
  while ((!result1 || !result2) && !std::empty(stacks[0])) {
    for (auto position: stacks[0]) {
      auto [x, y] = position;
      if (position == start) {
        result1 = time;
      }
      if (!result2 && grid[y][x] == 'a') {
        result2 = time;
      }
      for (auto [dx, dy]: directions) {
        auto x1 = x + dx;
        auto y1 = y + dy;
        if (0 <= y1 && y1 < height && 0 <= x1 && x1 < width) {
          if (!visited[y1 * width + x1] && grid[y][x] - grid[y1][x1] <= 1) {
            visited[y1 * width + x1] = true;
            stacks[1].push_back(std::make_tuple(x1, y1));
          }
        }
      }
    }
    stacks[0] = std::move(stacks[1]);
    ++time;
  }

  std::printf("Part 1 result %d\nPart 2 result %d\n", result1, result2);
  std::fflush(stdout);
}

int main() {
  constexpr int day = 12;
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
