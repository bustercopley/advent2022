#include "precompiled.hpp"

void solve(std::istream &stream) {
  int result1{};
  int result2{};

  std::vector<std::string> trees;
  for (std::string line; std::getline(stream, line);) { trees.push_back(line); }
  result1 = 2 * (std::size(trees) + std::size(trees[0]) - 2);
  for (int i = 1; i != (int)std::size(trees) - 1; ++i) {
    for (int j = 1; j != (int)std::size(trees[i]) - 1; ++j) {

      auto u1 = [&]() {
        for (int k = 0; k != i; ++k) {
          if (trees[i - k - 1][j] >= trees[i][j]) {
            return false;
          }
        }
        return true;
      };

      auto l1 = [&]() {
        for (int k = 0; k != j; ++k) {
          if (trees[i][j - k - 1] >= trees[i][j]) {
            return false;
          }
        }
        return true;
      };

      auto d1 = [&]() {
        for (int k = i + 1; k != (int)std::size(trees); ++k) {
          if (trees[k][j] >= trees[i][j]) {
            return false;
          }
        }
        return true;
      };

      auto r1 = [&]() {
        for (int k = j + 1; k != (int)std::size(trees[i]); ++k) {
          if (trees[i][k] >= trees[i][j]) {
            return false;
          }
        }
        return true;
      };

      auto u2 = [&]() {
        for (int k = 0; k != i; ++k) {
          if (trees[i - k - 1][j] >= trees[i][j]) {
            return k + 1;
          }
        }
        return i;
      };

      auto l2 = [&]() {
        for (int k = 0; k != j; ++k) {
          if (trees[i][j - k - 1] >= trees[i][j]) {
            return k + 1;
          }
        }
        return j;
      };

      auto d2 = [&]() {
        for (int k = i + 1; k != (int)std::size(trees); ++k) {
          if (trees[k][j] >= trees[i][j]) {
            return k - i;
          }
        }
        return (int)std::size(trees) - i - 1;
      };

      auto r2 = [&]() {
        for (int k = j + 1; k != (int)std::size(trees[i]); ++k) {
          if (trees[i][k] >= trees[i][j]) {
            return k - j;
          }
        }
        return (int)std::size(trees[i]) - j - 1;
      };

      result1 += u1() || d1() || l1() || r1();
      result2 = std::max(result2, u2() * l2() * d2() * r2());
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
