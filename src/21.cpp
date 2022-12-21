#include "precompiled.hpp"
#include <gmpxx.h>

auto regex = re::regex(R"(^(\w+): (?:(-?\d+)|(\w+) (.) (\w+))$)");

struct node {
  char op;
  mpq_class value;
  int children[2];
};

void solve(std::istream &stream) {
  symbols names;
  std::vector<node> nodes;

  for (std::string line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      auto index = names[match_string(m, 1)];
      nodes.resize(std::max(std::size(nodes), index + 1));
      auto &node = nodes[index];
      if (matched(m, 2)) {
        node.op = '\0';
        node.value = string_to<int>(match_view(m, 2, line));
      } else {
        node.op = match_view(m, 4, line)[0];
        node.children[0] = (int)names[match_string(m, 3)];
        node.children[1] = (int)names[match_string(m, 5)];
      }
    }
  }

  int root = (int)names["root"];
  if (nodes[root].op) {
    std::vector<int> stack{{root}};
    while (!stack.empty()) {
      auto &[op, value, children] = nodes[stack.back()];
      auto [a, b] = children;
      if (nodes[a].op == '\0' && nodes[b].op == '\0') {
        switch (op) {
        case '+':
          value = nodes[a].value + nodes[b].value;
          break;
        case '-':
          value = nodes[a].value - nodes[b].value;
          break;
        case '*':
          value = nodes[a].value * nodes[b].value;
          break;
        case '/': // divide
          value = nodes[a].value / nodes[b].value;
          break;
        default:
          std::exit(1);
        }
        op = '\0';
        stack.pop_back();
      } else {
        if (nodes[a].op) {
          stack.push_back(a);
        }
        if (nodes[b].op) {
          stack.push_back(b);
        }
      }
    }
  }
  gmp_printf("Part 1 result %Qd\n", nodes[root].value.get_mpq_t());
}

int main() {
  constexpr int day = 21;
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
