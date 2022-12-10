#include "precompiled.hpp"

auto regex = re::regex(R"(^addx (-?\d+)$|^noop$)");

enum class opcode { add, nop };
struct insn {
  opcode op;
  std::int64_t arg;
};

void solve(std::istream &stream) {
  std::vector<insn> program;
  for (std::string line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      if (matched(m, 1)) {
        auto arg = string_to<int>(match_view(m, 1, line));
        program.push_back(insn{opcode::add, arg});
      } else {
        program.push_back(insn{opcode::nop, 0});
      }
    }
  }

  int reg{1};
  int cycle{};
  int result1{};
  std::vector<std::string> screen(6, std::string(40, '.') + '\n');
  for (auto [op, arg]: program) {
    if (op == opcode::add) {
      for (int i = 0; i != 2; ++i) {
        if (std::abs(reg - (cycle % 40)) <= 1) {
          screen[cycle / 40][cycle % 40] = '#';
        }
        ++cycle;
        if ((cycle + 20) % 40 == 0) {
          result1 += cycle * reg;
        }
      }
      reg += arg;
    } else {
      if (std::abs(reg - (cycle % 40)) <= 1) {
        screen[cycle / 40][cycle % 40] = '#';
      }
      ++cycle;
      if ((cycle + 20) % 40 == 0) {
        result1 += cycle * reg;
      }
    }
  }
  std::printf("Part 1 result %d\nPart 2 result\n\n", result1);
  for (const auto &line: screen) { std::printf("%s", std::data(line)); }
}

int main() {
  constexpr int day = 10;
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
