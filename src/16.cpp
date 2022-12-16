#include "precompiled.hpp"

auto regex = re::regex(
  R"(^Valve (\w+) has flow rate=(\d+); tunnels? leads? to valves? (.*)$)");

auto regex2 = re::regex(R"(^(\w+)(?:, )?(.*)$)");

auto get_shortest_paths(const std::vector<std::vector<int>> &tunnels) {
  auto size = (int)std::size(tunnels);
  std::vector<std::vector<int>> paths(size,
    std::vector<int>(size, std::numeric_limits<int>::max() / 2));

  for (int i{}; auto js: tunnels) {
    for (int j: js) { paths[i][j] = 1; }
    paths[i][i] = 0;
    ++i;
  }
  // https://en.wikipedia.org/wiki/Floyd-Warshall_algorithm
  for (int k{}; k != size; ++k) {
    for (int i{}; i != size; ++i) {
      for (int j{}; j != size; ++j) {
        paths[i][j] = std::min(paths[i][j], paths[i][k] + paths[k][j]);
      }
    }
  }
  return paths;
}

auto part1(int position, int time, auto &valves, const auto &paths,
  const auto &rates) -> int {

  int score{};
  for (int dst{}; auto t: paths[position]) {
    if (rates[dst] && !valves[dst] && time >= t + 1) {
      valves[dst] = true;
      int candidate = part1(dst, time - t - 1, valves, paths, rates);
      valves[dst] = false; // backtrack
      score = std::max(score, candidate + (time - t - 1) * rates[dst]);
    }
    ++dst;
  }
  return score;
}

auto part2(int p1, int p2, int t1, int t2, auto &valves, const auto &paths,
  const auto &rates) -> int {

  int score{};
  for (int dst{}; auto t: paths[p1]) {
    if (rates[dst] && !valves[dst] && t1 >= t + 1) {
      valves[dst] = true;
      // For the next turn the players' roles are reversed
      int candidate = part2(p2, dst, t2, t1 - t - 1, valves, paths, rates);
      valves[dst] = false; // backtrack
      score = std::max(score, candidate + (t1 - t - 1) * rates[dst]);
    }
    ++dst;
  }
  return score;
}

void solve(std::istream &stream) {
  symbols names;
  std::vector<int> rates;
  std::vector<std::vector<int>> tunnels;

  // Parse input
  for (std::string line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      int src = names[match_string(m, 1)];
      auto dsts = match_view(m, 3, line);
      for (auto [name]: split(dsts, regex2)) {
        int dst = names[std::string{name}];
        tunnels.resize(std::max(names.size(), tunnels.size()));
        tunnels[src].push_back(dst);
      }
      rates.resize(std::max(names.size(), rates.size()));
      rates[src] = string_to<int>(match_view(m, 2, line));
    }
  }
  auto paths = get_shortest_paths(tunnels);

  // Initialize all valves to 'off'
  std::vector<bool> valves(names.size());

  // Find best scores recursively
  auto result1 = part1(names["AA"], 30, valves, paths, rates);
  auto result2 = part2(names["AA"], names["AA"], 26, 26, valves, paths, rates);
  std::printf("Part 1 result %d\n"
              "Part 2 result %d\n",
    result1, result2);
}

int main() {
  constexpr int day = 16;
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
