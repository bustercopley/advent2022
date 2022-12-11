#include "precompiled.hpp"

auto regex = re::regex(R"(^Monkey \d+:$)"
                       R"(|^  Starting items: (.*)$)"
                       R"(|^  Operation: new = old \* (\d+)$)"
                       R"(|^  Operation: new = old \+ (\d+)$)"
                       R"(|^  Operation: new = old \* (old)$)"
                       R"(|^  Test: divisible by (\d+)$)"
                       R"(|^    If true: throw to monkey (\d+)$)"
                       R"(|^    If false: throw to monkey (\d+)$)"
                       R"(|^$)");

auto regex2 = re::regex(R"((\d+)((?:,(.*))?))");

enum opcode { multiply, add, square };

struct monkey {
  std::vector<std::int64_t> items;
  opcode op{};
  int arg{};
  int test{};
  int targets[2]{};
  std::int64_t inspections{};
};

void solve(std::istream &stream) {
  std::vector<monkey> monkeys;
  for (std::string line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        monkeys.emplace_back();
        for (auto [s]: split(match_view(m, 1, line), regex2)) {
          monkeys.back().items.push_back(string_to<std::int64_t>(s));
        }
      } else if (matched(m, 2)) {
        monkeys.back().op = opcode::multiply;
        monkeys.back().arg = string_to<int>(match_view(m, 2, line));
      } else if (matched(m, 3)) {
        monkeys.back().op = opcode::add;
        monkeys.back().arg = string_to<int>(match_view(m, 3, line));
      } else if (matched(m, 4)) {
        monkeys.back().op = opcode::square;
      } else if (matched(m, 5)) {
        monkeys.back().test = string_to<int>(match_view(m, 5, line));
      } else if (matched(m, 6)) {
        monkeys.back().targets[0] = string_to<int>(match_view(m, 6, line));
      } else if (matched(m, 7)) {
        monkeys.back().targets[1] = string_to<int>(match_view(m, 7, line));
      }
    }
  }

  std::int64_t results[2]{};
  std::int64_t modulus = 1;
  const int N = std::size(monkeys);
  for (auto &m: monkeys) { modulus *= m.test; }
  int days[2][2]{{3, 20}, {1, 10000}};
  for (int day = 0; const auto [divisor, repeats]: days) {
    std::vector<monkey> monkeys1;
    std::ranges::copy(monkeys, std::back_inserter(monkeys1));
    for (int i = 0; i != repeats * N; ++i) {
      monkey &m = monkeys1[i % N];
      m.inspections += std::size(m.items);
      for (auto item: m.items) {
        switch (m.op) {
        case opcode::multiply:
          item *= m.arg;
          break;
        case opcode::add:
          item += m.arg;
          break;
        case opcode::square:
          item *= item;
          break;
        }
        item /= divisor;
        item %= modulus;
        if (item % m.test == 0) {
          monkeys1[m.targets[0]].items.push_back(item);
        } else {
          monkeys1[m.targets[1]].items.push_back(item);
        }
      }
      m.items.clear();
    }
    std::ranges::nth_element(monkeys1, std::next(std::ranges::begin(monkeys1)),
      std::greater<>{}, [](const auto &a) { return a.inspections; });
    results[day++] = monkeys1[0].inspections * monkeys1[1].inspections;
  }

  std::printf("Part 1 result %lld\n"
              "Part 2 result %lld\n",
    results[0], results[1]);
}

int main() {
  constexpr int day = 11;
  std::printf("Day %d\n", day);
  for (auto pattern: {"test/%02d", "test/%02d-2", "input/%02d"}) {
    auto filename = string_printf(pattern, day);
    if (std::ifstream stream(std::data(filename)); stream) {
      std::printf("\nReading file %s\n", std::data(filename));
      solve(stream);
    }
  }

  return 0;
}
