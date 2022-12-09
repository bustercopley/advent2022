#include "precompiled.hpp"

constexpr std::tuple<int, int> directions[]{{0, -1}, {-1, 0}, {0, 1}, {1, 0}};

void solve(std::istream &stream) {
  std::vector<std::string> trees;
  for (std::string line; std::getline(stream, line);) { trees.push_back(line); }
  int h = std::size(trees);
  int w = std::size(trees[0]);
  int result1{2 * h + 2 * w - 4};
  int result2{};

  for (int y = 1; y != h - 1; ++y) {
    for (int x = 1; x != w - 1; ++x) {

      auto next = [=](auto x, auto y, auto direction) {
        auto [dx, dy] = direction;
        auto x1 = x + dx;
        auto y1 = y + dy;
        auto ok = 0 <= x1 && x1 < w && 0 <= y1 && y1 < h;
        return std::make_tuple(x1, y1, ok);
      };

      auto is_visible = [=](auto direction) {
        for (auto [x1, y1, ok] = next(x, y, direction); ok;
             std::tie(x1, y1, ok) = next(x1, y1, direction)) {
          if (trees[y1][x1] >= trees[y][x]) {
            return false;
          }
        }
        return true;
      };

      auto get_score = [=](auto direction) {
        int score = 0;
        for (auto [x1, y1, ok] = next(x, y, direction); ok;
             std::tie(x1, y1, ok) = next(x1, y1, direction)) {
          ++score;
          if (trees[y1][x1] >= trees[y][x]) {
            break;
          }
        }
        return score;
      };

      bool visible = false;
      int score = 1;
      for (auto direction: directions) {
        visible = visible || is_visible(direction);
        score = score * get_score(direction);
      }
      result1 += visible;
      result2 = std::max(result2, score);
    }
  }
  std::printf("Part 1 result %d\n"
              "Part 2 result %d\n",
    result1, result2);
}

int main() {
  constexpr int day = 8;
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
