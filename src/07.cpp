#include "precompiled.hpp"

auto regex = re::regex(R"(^\$ cd (.*)$|^dir (.*)$|^(\d+) (.*)$)");

struct dirent {
  std::string name;
  bool is_dir;
  int size = -1;
  int parent;
  std::vector<int> children;
};

std::uint64_t total_size(int id, std::vector<dirent> &table) {
  if (table[id].is_dir) {
    if (table[id].size == -1) {
      std::uint64_t size = 0;
      for (auto child: table[id].children) { size += total_size(child, table); }
      table[id].size = size;
    }
  }
  return table[id].size;
}

void solve(std::istream &stream) {
  std::uint64_t result1{};
  std::uint64_t result2{};

  std::vector<dirent> table;
  auto mknode = [&](dirent d) {
    table.push_back(d);
    return (int)std::size(table) - 1;
  };

  int root = mknode(dirent{"root", true, -1, -1, {}});
  int pwd = root;

  for (std::string line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        auto name = match_view(m, 1, line);
        if (name == "/") {
          pwd = root;
        } else if (name == "..") {
          pwd = table[pwd].parent;
        } else {
          for (auto id: table[pwd].children) {
            if (table[id].name == name) {
              pwd = id;
              break;
            }
          }
        }
      } else if (matched(m, 2)) {
        auto name = match_view(m, 2, line);
        auto id = mknode(dirent{std::string(name), true, -1, pwd, {}});
        table[pwd].children.push_back(id);
      } else if (matched(m, 4)) {
        auto name = match_view(m, 4, line);
        auto size = string_to<int>(match_view(m, 3, line));
        auto id = mknode(dirent{std::string(name), false, size, pwd, {}});
        table[pwd].children.push_back(id);
      }
    }
  }

  for (int index{}; [[maybe_unused]] auto &d: table) {
    if (index != root && d.is_dir) {
      auto size = total_size(index, table);
      if (size <= 100000) {
        result1 += size;
      }
    }
    ++index;
  }

  result2 = std::numeric_limits<std::uint64_t>::max();
  auto needed = total_size(root, table) - 40000000;
  for (int index{}; [[maybe_unused]] auto &d: table) {
    if (d.is_dir) {
      auto size = total_size(index, table);
      if (size >= needed && size < result2) {
        result2 = size;
      }
    }
    ++index;
  }

  std::printf("Part 1 result %llu\n"
              "Part 2 result %llu\n",
    result1, result2);
}

int main() {
  constexpr int day = 7;
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
