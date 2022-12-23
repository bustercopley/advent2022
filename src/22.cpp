#include "precompiled.hpp"
#include <cmath>

auto regex = re::regex(R"((\d+)([LR]?)(.*))");
constexpr std::pair<int, int> facings[]{{1, 0}, {0, -1}, {-1, 0}, {0, 1}};

template <typename T> struct matrix2 {
  matrix2(int m, int n): n(n), data(m * n) {}
  matrix2(int m, int n, T init): n(n), data(m * n, init) {}
  auto &operator[](int x, int y) { return data[n * y + x]; }
  auto operator[](int x, int y) const { return data[n * y + x]; }

private:
  int n;
  std::vector<T> data;
};

struct crease {
  int face;
  int rotation;
};

void mmul(auto &ab, const auto &a, const auto &b) {
  for (int i = 0; i != 3; ++i) {
    for (int j = 0; j != 3; ++j) {
      ab[i][j] = 0;
      for (int k = 0; k != 3; ++k) { ab[i][j] += a[i][k] * b[k][j]; }
    }
  }
}

void mmul_transpose(auto &ab, const auto &a, const auto &b) {
  for (int i = 0; i != 3; ++i) {
    for (int j = 0; j != 3; ++j) {
      ab[i][j] = 0;
      for (int k = 0; k != 3; ++k) { ab[i][j] += a[i][k] * b[j][k]; }
    }
  }
}

void solve(std::istream &stream) {
  int results[2]{};
  std::vector<std::string> grid;
  std::vector<std::tuple<int, char>> directions;
  int surface_area{};
  int max_width{};
  for (std::string line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      for (auto [n, d]: split<2>(line, regex)) {
        char turn = std::empty(d) ? '*' : d[0];
        directions.emplace_back(string_to<int>(n), turn);
      }
    } else if (!std::empty(line)) {
      grid.push_back(line);
      max_width = std::max(max_width, (int)std::size(line));
      surface_area += std::size(line) - std::ranges::count(line, ' ');
    }
  }
  int side = std::sqrt(surface_area / 6);
  int net_height = std::size(grid) / side;
  int net_width = max_width / side;

  // Copy each face into a square matrix (and reverse y)
  std::vector<matrix2<char>> faces;
  std::vector<std::tuple<int, int>> positions;
  for (int i = 0; i != (int)std::size(grid) / side; ++i) {
    for (int j = 0; j != (int)std::size(grid[side * i]) / side; ++j) {
      if (grid[side * i][side * j] != ' ') {
        auto &face = faces.emplace_back(side, side);
        positions.emplace_back(j, net_height - 1 - i);
        for (int x = 0; x != side; ++x) {
          for (int y = 0; y != side; ++y) {
            face[x, side - 1 - y] = grid[side * i + y][side * j + x];
          }
        }
      }
    }
  }

  // Build index from position of face to index of face
  std::map<std::tuple<int, int>, int> position_face;
  for (int face{}; auto position: positions) {
    position_face[position] = face++;
  }

  // If crease[is_part_one][face][facing] = {face1, facing1},
  // then walking past an edge of face 'face' in direction 'facing' warps
  // to face 'face1' facing in direction 'facing1'
  crease creases[2][6][4]{};

  // Build creases for part one
  for (int face = 0; face != 6; ++face) {
    auto [x, y] = positions[face];
    for (int facing = 0; facing != 4; ++facing) {
      auto [fx, fy] = facings[facing];
      int x1 = x;
      int y1 = y;
      while (true) {
        x1 = (x1 + fx + net_width) % net_width;
        y1 = (y1 + fy + net_height) % net_height;
        auto iter = position_face.find(std::make_tuple(x1, y1));
        if (iter != std::end(position_face)) {
          auto &crease = creases[0][face][facing];
          crease.face = iter->second;
          crease.rotation = 0;
          break;
        }
      }
    }
  }

  // Build face orientations for part two
  bool done[6]{true, false, false, false, false, false};
  int orientations[6][3][3]{{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}};
  while (true) {
    // Find a face that is done and has a neighbour in the net that is not done
    int face{};
    int facing{};
    int face1{};
    while (face != 6) {
      while (face != 6 && !done[face]) ++face;
      auto [x, y] = positions[face];
      auto [fx, fy] = facings[facing];
      int x1 = x + fx;
      int y1 = y + fy;
      if (x1 >= 0 && x1 < net_width && y1 >= 0 && y1 < net_height) {
        auto iter = position_face.find(std::make_tuple(x1, y1));
        if (iter != std::end(position_face) && !done[iter->second]) {
          face1 = iter->second;
          break;
        }
      }
      ++facing;
      if (facing == 4) {
        ++face;
        facing = 0;
      }
    }
    if (face == 6) {
      break;
    }

    // Can walk from 'face' in direction 'facing' to 'face1' on the net
    done[face1] = true;
    const auto [fx, fy] = facings[facing];
    // Matrix b of the rotation for going over the crease
    int b[3][3]{{fy * fy, 0, -fx}, {0, fx * fx, -fy}, {fx, fy, 0}};
    mmul(orientations[face1], b, orientations[face]);
  }

  // Build creases for part two
  for (int face = 0; face != 6; ++face) {
    const auto &m = orientations[face];
    for (int facing = 0; facing != 4; ++facing) {
      auto [fx, fy] = facings[facing];
      // Matrix b of the rotation for going over the crease
      int b[3][3]{{fy * fy, 0, -fx}, {0, fx * fx, -fy}, {fx, fy, 0}};
      int bm[3][3]{};
      mmul(bm, b, m);
      int face1 = 0;
      while (std::memcmp(orientations[face1][2], bm[2], sizeof bm[2])) {
        ++face1;
      }
      const auto &m1 = orientations[face1];
      int a[3][3]{};
      mmul_transpose(a, bm, m1);
      // Nice sign error
      auto f = std::make_pair(a[0][0], -a[1][0]);
      int facing1 = std::ranges::find(facings, f) - std::begin(facings);
      creases[1][face][facing].face = face1;
      creases[1][face][facing].rotation = facing1;
    }
  }

  // Follow the directions
  for (int is_part_two = 0; is_part_two != 2; ++is_part_two) {
    int face = 0;
    int x = 0;
    int y = side - 1;
    int facing = 0;

    for (auto [steps, d]: directions) {
      for (int step = 0; step != steps; ++step) {
        auto [fx, fy] = facings[facing];
        int face1 = face;
        int facing1 = facing;
        int x1 = x + fx;
        int y1 = y + fy;
        if (x1 < 0 || y1 < 0 || x1 >= side || y1 >= side) {
          auto [fprime, rotation] = creases[is_part_two][face][facing];
          face1 = fprime;
          facing1 = (facing + rotation) % 4;
          x1 = (x1 + side) % side;
          y1 = (y1 + side) % side;
          for (int i = 0; i != rotation; ++i) {
            std::tie(x1, y1) = std::make_pair(y1, side - 1 - x1);
          }
        }
        if (faces[face1][x1, y1] == '#') {
          break;
        }
        face = face1;
        facing = facing1;
        x = x1;
        y = y1;
      }
      facing = (facing + (d == 'R') - (d == 'L') + 4) % 4;
    }
    auto [n, m] = positions[face];
    int i = (net_height - 1 - m) * side + (side - 1 - y);
    int j = n * side + x;
    results[is_part_two] = 1000 * (i + 1) + 4 * (j + 1) + facing;
  }

  std::printf("Part 1 result %d\n"
              "Part 2 result %d\n",
    results[0], results[1]);
}

int main() {
  constexpr int day = 22;
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
