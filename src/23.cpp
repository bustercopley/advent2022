#include "precompiled.hpp"

using point = std::pair<int, int>;
std::array<point, 3> directions[]{
  {{{0, 1}, {-1, 1}, {1, 1}}},
  {{{0, -1}, {-1, -1}, {1, -1}}},
  {{{-1, 0}, {-1, -1}, {-1, 1}}},
  {{{1, 0}, {1, -1}, {1, 1}}},
};
point neighbors[]{
  {-1, -1},
  {-1, 0},
  {-1, 1},
  {0, -1},
  {0, 1},
  {1, -1},
  {1, 0},
  {1, 1},
};

point add(point a, point b) {
  auto [ax, ay] = a;
  auto [bx, by] = b;
  return point{ax + bx, ay + by};
}

void solve(std::istream &stream) {
  int result1{};
  int result2{};
  std::set<std::pair<int, int>> grid;
  int y = 0;
  for (std::string line; std::getline(stream, line);) {
    for (int x = 0; char c: line) {
      if (c == '#') {
        grid.emplace(x, y);
      }
      ++x;
    }
    --y;
  }

  int turns = 0;
  while (true) {
    std::map<std::pair<int, int>, std::set<std::pair<int, int>>> proposals;
    std::set<std::pair<int, int>> ngrid;
    for (auto from: grid) {
      bool proposed{};
      for (auto d: neighbors) {
        if (grid.contains(add(from, d))) {
          for (int j = 0; j != 4; ++j) {
            auto [to, a, b] = directions[(turns + j) % 4];
            if (!grid.contains(add(from, to)) && !grid.contains(add(from, a)) &&
              !grid.contains(add(from, b))) {
              proposals[add(from, to)].insert(from);
              proposed = true;
              break;
            }
          }
          break;
        }
      }
      if (!proposed) {
        ngrid.insert(from);
      }
    }

    int elves_moved{};

    for (const auto &[to, from_set]: proposals) {
      if (std::size(from_set) == 1) {
        ngrid.insert(to);
        ++elves_moved;
      } else {
        for (auto from: from_set) { ngrid.insert(from); }
      }
    }

    grid = std::move(ngrid);
    ++turns;

    if (turns == 10) {
      auto minx = std::numeric_limits<int>::max();
      auto miny = std::numeric_limits<int>::max();
      auto maxx = std::numeric_limits<int>::min();
      auto maxy = std::numeric_limits<int>::min();
      for (auto p: grid) {
        auto [x, y] = p;
        minx = std::min(minx, x);
        miny = std::min(miny, y);
        maxx = std::max(maxx, x);
        maxy = std::max(maxy, y);
      }
      result1 = (maxx - minx + 1) * (maxy - miny + 1) - std::size(grid);
    }
    if (!elves_moved) {
      result2 = turns;
    }
    if (result1 && result2) {
      break;
    }
  }

  std::printf("Part 1 result %d\n"
              "Part 2 result %d\n",
    result1, result2);
}

int main() {
  constexpr int day = 23;
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
