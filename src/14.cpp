#include "precompiled.hpp"

auto regex = re::regex(R"((\d+),(\d+)(?: -> )?(.*)$)");

void solve(std::istream &stream) {
  std::map<std::tuple<int, int>, char> map;
  int minx = 500;
  int miny = 0;
  int maxx = 500;
  int maxy = 0;

  auto is_open = [&](int x, int y) -> bool {
    auto iter = map.find(std::make_tuple(x, y));
    return iter == std::end(map);
  };

  auto insert = [&](int x, int y, char c) {
    map[std::make_tuple(x, y)] = c;
    minx = std::min(minx, x);
    miny = std::min(miny, y);
    maxx = std::max(maxx, x);
    maxy = std::max(maxy, y);
  };

  for (std::string line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      int x = string_to<int>(match_view(m, 1, line));
      int y = string_to<int>(match_view(m, 2, line));
      insert(x, y, '#');
      for (auto [s1, t1]: split<2>(match_view(m, 3, line), regex)) {
        auto x1 = string_to<int>(s1);
        auto y1 = string_to<int>(t1);
        auto target = std::make_tuple(x1, y1);
        while (std::make_tuple(x, y) != target) {
          x += (x1 > x) - (x1 < x);
          y += (y1 > y) - (y1 < y);
          insert(x, y, '#');
        }
      }
    }
  }

  int floor = maxy + 1;
  auto walls = map;

  for (int part = 0; part != 2; ++part) {
    if (part) {
      map = std::move(walls);
    }
    constexpr auto source = std::make_tuple(500, 0);
    constexpr int at_rest = 0;
    constexpr int falling = 1;
    constexpr int halt = 2;
    int result = 0;
    int state = at_rest;
    std::vector<std::tuple<int, int>> path;
    std::tuple<int, int> position;
    auto &[x, y] = position;
    while (state != halt) {
      if (state == at_rest) {
        position = std::empty(path) ? source : path.back();
        state = falling;
      }
      if (state == falling) {
        if (part == 0 && y > maxy) {
          state = halt;
        } else {
          if ((part == 0 || y < floor) && is_open(x, y + 1)) {
            ++y;
            path.push_back(position);
          } else if ((part == 0 || y < floor) && is_open(x - 1, y + 1)) {
            ++y;
            --x;
            path.push_back(position);
          } else if ((part == 0 || y < floor) && is_open(x + 1, y + 1)) {
            ++y;
            ++x;
            path.push_back(position);
          } else {
            insert(x, y, 'o');
            state = at_rest;
            path.pop_back();
            if (position == source) {
              state = halt;
            }
            ++result;
          }
        }
      }
    }
    std::printf("Part %d result %d\n", part + 1, result);
  }
}

int main() {
  constexpr int day = 14;
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
