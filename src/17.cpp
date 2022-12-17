#include "precompiled.hpp"

const std::vector<std::pair<int, int>> models[]{
  {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
  {{1, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 2}},
  {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}},
  {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
  {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
};

const int model_heights[]{1, 3, 3, 4, 2};

struct rock_tower {
  rock_tower(const std::string &jets): jets_(jets){};

  void add_block() {
    // Block appears
    int h = height();
    int x = 2;
    int y = h + 3;

    // Ensure empty rows
    rows_.resize(y + model_heights[model_index_]);

    while (true) {
      int dx = (jets_[jet_index_] == '>') - (jets_[jet_index_] == '<');
      jet_index_ = (jet_index_ + 1) % std::size(jets_);
      if (!detect_collision(x + dx, y)) {
        // Jet pushes block
        x = x + dx;
      }
      if (!detect_collision(x, y - 1)) {
        // Block moves down
        y = y - 1;
      } else {
        // Block comes to rest
        for (auto [mx, my]: models[model_index_]) {
          rows_[y + my][x + mx] = true;
        }
        model_index_ = (model_index_ + 1) % std::size(models);
        return;
      }
    }
  }

  int add_blocks(int count) {
    int previous_height = height();
    for (int i = 0; i != count; ++i) { add_block(); }
    return height() - previous_height;
  }

  bool detect_collision(int x, int y) {
    for (auto [mx, my]: models[model_index_]) {
      if (0 > x + mx || x + mx >= 7 || 0 > y + my || rows_[y + my][x + mx]) {
        return true;
      }
    }
    return false;
  }

  int height() {
    // Discount empty rows at top
    for (auto h = std::size(rows_);
         auto &row: std::ranges::views::reverse(rows_)) {
      constexpr std::array<bool, 7> empty_row{};
      if (row != empty_row) {
        return h;
      }
      --h;
    }
    return 0;
  }

  int model_index_{};
  int jet_index_{};
  std::string jets_;
  std::vector<std::array<bool, 7>> rows_;
};

void solve(const std::string &jets) {
  int result1{};
  std::int64_t result2{};
  // Part 1
  {
    rock_tower tower(jets);
    result1 = tower.add_blocks(2022);
  }

  // Part 2
  constexpr std::int64_t limit = 1000000000000;
  int init = limit % std::size(jets) + 3 * std::size(jets);
  rock_tower tower(jets);
  tower.add_blocks(init);
  std::int64_t init_height = tower.height();
  std::vector<int> differences;
  while (true) {
    for (int i = 0; i != 2; ++i) {
      int delta_h = tower.add_blocks(std::size(jets));
      differences.push_back(delta_h);
    }
    int mid = std::size(differences) / 2;
    int i = 0;
    while (i != mid) {
      if (differences[mid + i] != differences[i]) {
        break;
      }
      ++i;
    }
    if (i == mid) {
      // Found a cycle
      differences.resize(mid);
      std::int64_t cycle_time = mid * std::size(jets);
      std::int64_t cycle_height = std::accumulate(std::begin(differences),
        std::end(differences), std::int64_t{});
      std::int64_t remaining = limit - init;
      std::int64_t cycles = remaining / cycle_time;
      std::int64_t remainder = remaining % cycle_time;
      std::int64_t height = init_height + cycles * cycle_height;
      int i = 0;
      while (remainder > 0) {
        height += differences[i++];
        remainder -= std::size(jets);
      }
      result2 = height;
      break;
    }
  }

  std::printf("Part 1 result %d\n"
              "Part 2 result %lld\n",
    result1, result2);
  std::fflush(stdout);
}

int main() {
  constexpr int day = 17;
  std::printf("Day %d\n", day);
  for (auto pattern: {"test/%02d", "input/%02d"}) {
    auto filename = string_printf(pattern, day);
    if (std::ifstream stream(std::data(filename)); stream) {
      std::printf("\nReading file %s\n", std::data(filename));
      // Just one line today
      for (std::string line; std::getline(stream, line);) {
        solve(line);
        break;
      }
    }
  }

  return 0;
}
