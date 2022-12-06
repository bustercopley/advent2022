#include "precompiled.hpp"

auto regex = re::regex(R"((\[.\])|^move (\d+) from (\d+) to (\d+)$)");

void solve(std::istream &stream) {
  std::vector<std::vector<char>> stacks1, stacks2;

  for (std::string line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        std::size_t j = 0;
        for (std::size_t i = 1; i < std::size(line); i += 4) {
          if (line[i] != ' ') {
            stacks1.resize(std::max(std::size(stacks1), j + 1));
            stacks1[j].push_back(line[i]);
          }
          ++j;
        }
      } else {
        // Before starting the moves, reverse each stack and make a copy
        if (std::empty(stacks2)) {
          for (auto &stack: stacks1) { std::ranges::reverse(stack); }
          stacks2 = stacks1;
        }

        auto a = string_to<int>(match_view(m, 2, line));
        auto b = string_to<int>(match_view(m, 3, line));
        auto c = string_to<int>(match_view(m, 4, line));

        // Part 1
        {
          auto end = std::end(stacks1[b - 1]);
          auto iter = end;
          for (int i = 0; i != a; ++i) { stacks1[c - 1].push_back(*--iter); }
          stacks1[b - 1].erase(iter, end);
        }

        // Part 2
        {
          auto end = std::end(stacks2[b - 1]);
          auto iter = std::prev(end, a);
          std::copy(iter, end, std::back_inserter(stacks2[c - 1]));
          stacks2[b - 1].erase(iter, end);
        }
      }
    }
  }

  for (const auto &stacks: {stacks1, stacks2}) {
    for (const auto &stack: stacks) { std::printf("%c", stack.back()); }
    std::printf("\n");
  }
}

int main() {
  constexpr int day = 5;
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
