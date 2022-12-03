#include "precompiled.hpp"

void solve(std::istream &stream) {
  std::uint64_t result1{};
  std::uint64_t result2{};

  int rucksacks[3][52]{};
  int elf{};

  for (std::string line; std::getline(stream, line);) {
    int compartment_size = std::size(line) / 2;
    std::memset(&rucksacks[elf], '\0', sizeof rucksacks[elf]);
    for (int i = 0; int c: line) {
      auto item_type = c > 'a' ? c - 'a' : c - 'A' + 26;
      if (0 <= item_type && item_type < 52) {
        rucksacks[elf][item_type] |= (i < compartment_size ? 1 : 2);
      }
      ++i;
    }
    for (int i = 0; i != 52; ++i) {
      if (rucksacks[elf][i] == 3) {
        result1 += i + 1;
      }
    }
    if (elf == 2) {
      for (int i = 0; i != 52; ++i) {
        if (rucksacks[0][i] && rucksacks[1][i] && rucksacks[2][i]) {
          result2 += i + 1;
        }
      }
    }
    elf = (elf + 1) % 3;
  }

  std::printf("Part 1 result %llu\n"
              "Part 2 result %llu\n",
    result1, result2);
}

int main() {
  constexpr int day = 3;
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
