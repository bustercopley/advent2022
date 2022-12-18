#include "precompiled.hpp"

auto regex = re::regex(R"(^(-?\d+),(-?\d+),(-?\d+)$)");

using point = std::tuple<int, int, int>;

constexpr point directions[]{
  {-1, 0, 0},
  {+1, 0, 0},
  {0, -1, 0},
  {0, +1, 0},
  {0, 0, -1},
  {0, 0, +1},
};

constexpr auto add(point p, point d) {
  auto [x, y, z] = p;
  auto [dx, dy, dz] = d;
  return point{x + dx, y + dy, z + dz};
};

constexpr auto negate(point d) {
  auto [dx, dy, dz] = d;
  return point{-dx, -dy, -dz};
};

constexpr auto orthogonal(point f, point d) {
  auto [fx, fy, fz] = f;
  auto [dx, dy, dz] = d;
  return fx * dx + fy * dy + fz * dz == 0;
};

void solve(std::istream &stream) {
  int total_surface_area{};
  int external_surface_area{};

  std::set<point> volume;
  int total_surface_area{};
  for (std::string line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      int x = string_to<int>(match_view(m, 1, line));
      int y = string_to<int>(match_view(m, 2, line));
      int z = string_to<int>(match_view(m, 3, line));
      total_surface_area += 6;
      for (auto [dx, dy, dz]: directions) {
        auto o = std::make_tuple(x + dx, y + dy, z + dz);
        if (volume.contains(o)) {
          total_surface_area -= 2;
        }
      }
      volume.insert(std::make_tuple(x, y, z));
    }
  }

  if (!std::empty(volume)) {
    // Start with the left-facing face of a leftmost cube
    auto face = std::make_pair(*volume.begin(), directions[0]);
    std::vector<std::pair<point, point>> stack{face};
    std::set<std::pair<point, point>> already{face};
    int external_surface_area{};
    while (!std::empty(stack)) {
      ++external_surface_area;
      auto [p, f] = stack.back();
      stack.pop_back();
      for (auto d: directions) {
        if (orthogonal(f, d)) {
          //        +----+
          //        |\    \
          //        | +----+     +--- +----+     +----+
          //   +----+ |    |    / d->/    /|    / d->/|
          //   |\ d->\|    |   +----+----+ |   +----+ |
          //   | +----+----+   |    |    | +   |    | +
          //   + |    |        |    |    |/    |    |/
          //    \|    |        +----+----+     +----+
          //     +----+
          std::pair<point, point> cases[]{
            {add(add(p, f), d), negate(d)},
            {add(p, d), f},
            {p, d},
          };
          for (auto face: cases) {
            auto [p1, _] = face;
            if (volume.contains(p1)) {
              if (auto [_, inserted] = already.insert(face); inserted) {
                stack.push_back(face);
              }
              break;
            }
          }
        }
      }
    }
  }

  std::printf("Part 1 result %d\n"
              "Part 2 result %d\n",
    total_surface_area, external_surface_area);
}

int main() {
  constexpr int day = 18;
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
