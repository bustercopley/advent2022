#include "precompiled.hpp"
#include <gmpxx.h>

auto regex = re::regex(R"(^(\w+): (?:(-?\d+)|(\w+) (.) (\w+))$)");

struct node {
  char op;
  mpq_class rs[2];
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
        node.rs[0] = string_to<int>(match_view(m, 2, line));
      } else {
        node.op = match_view(m, 4, line)[0];
        node.children[0] = (int)names[match_string(m, 3)];
        node.children[1] = (int)names[match_string(m, 5)];
      }
    }
  }

  auto reduce_node = [&](int index) -> bool {
    auto &[op, rs, children] = nodes[index];
    auto &a = nodes[children[0]];
    auto &b = nodes[children[1]];
    if (a.op == '\0' && b.op == '\0') {
      switch (op) {
      case '+':
        rs[0] = a.rs[0] + b.rs[0];
        rs[1] = a.rs[1] + b.rs[1];
        break;
      case '-':
        rs[0] = a.rs[0] - b.rs[0];
        rs[1] = a.rs[1] - b.rs[1];
        break;
      case '*':
        if (a.rs[1] != 0 && b.rs[1] != 0) {
          std::cout << "Quadratic term\n";
          std::exit(1);
        }
        rs[0] = a.rs[0] * b.rs[0];
        rs[1] = a.rs[0] * b.rs[1] + a.rs[1] * b.rs[0];
        break;
      case '/': // divide
        if (a.rs[1] != 0 && b.rs[1] != 0) {
          std::cout << "Reciprocal term\n";
          std::exit(1);
        }
        rs[0] = a.rs[0] / b.rs[0];
        rs[1] = a.rs[1] / b.rs[0];
        break;
      default:
        std::exit(1);
      }
      op = '\0';
      return true;
    }
    return false;
  };

  int root = (int)names["root"];
  int humn = (int)names["humn"];
  mpq_class humn_value = nodes[humn].rs[0];
  nodes[humn].rs[0] = 0;
  nodes[humn].rs[1] = 1;

  std::vector<int> stack{{nodes[root].children[0], nodes[root].children[1]}};
  while (!stack.empty()) {
    if (reduce_node(stack.back())) {
      stack.pop_back();
    } else {
      for (int i = 0; i != 2; ++i) {
        auto index = nodes[stack.back()].children[i];
        if (nodes[index].op) {
          stack.push_back(index);
        }
      }
    }
  }

  auto [l, r] = nodes[root].children;
  if (nodes[r].rs[1] != 0) {
    std::swap(l, r);
  }
  mpq_class result2 = (nodes[r].rs[0] - nodes[l].rs[0]) / nodes[l].rs[1];
  reduce_node(root);
  const auto &rs = nodes[root].rs;
  mpq_class result1 = rs[0] + rs[1] * humn_value;

  gmp_printf("Part 1 result %Qd\n" "Part 2 result %Qd\n", result1, result2);
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
