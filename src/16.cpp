#include "precompiled.hpp"

auto regex = re::regex(
  R"(^Valve (\w+) has flow rate=(\d+); tunnels? leads? to valves? (.*)$)");

auto regex2 = re::regex(R"(^(\w+)(?:, )?(.*)$)");

auto get_shortest_paths(const std::vector<std::vector<int>> &tunnels) {
  std::map<std::pair<int, int>, int> paths;
  std::vector<std::pair<int, int>> stacks[2];
  // Record all the zero-length paths and put them on the stack to be processed
  for (int src{}; auto tunnel: tunnels) {
    std::pair<int, int> path{src, src};
    stacks[0].push_back(path);
    paths.emplace(path, 0);
    ++src;
  }
  int time{};
  while (!std::empty(stacks[0])) {
    ++time;
    // Process all the paths from last time
    for (auto [src, dst]: stacks[0]) {
      for (auto next: tunnels[dst]) {
        std::pair<int, int> path{src, next};
        if (auto [_, emplaced] = paths.try_emplace(path, time); emplaced) {
          // Found a path with one more step; process it again next time
          stacks[1].push_back(path);
        }
      }
    }
    // Shift paths 'to be processed next time' into paths 'from last time'.
    stacks[0] = std::move(stacks[1]);
  }
  return paths;
}

auto part1(int position, int time, auto &valves, const auto &paths,
  const auto &rates) -> int {

  int score{};
  auto [begin, end] = paths.equal_range(position);
  for (auto [src1, path]: std::ranges::subrange(begin, end)) {
    auto [dst, t] = path;
    if (!valves[dst] && rates[dst] && time >= t + 1) {
      valves[dst] = true;
      int candidate = part1(dst, time - t - 1, valves, paths, rates);
      valves[dst] = false; // backtrack
      score = std::max(score, candidate + (time - t - 1) * rates[dst]);
    }
  }
  return score;
}

auto part2(int p1, int p2, int t1, int t2, auto &valves, const auto &paths,
  const auto &rates) -> int {

  int score{};
  auto [begin, end] = paths.equal_range(p1);
  for (auto [src1, path]: std::ranges::subrange(begin, end)) {
    auto [dst, t] = path;
    if (!valves[dst] && rates[dst] && t1 >= t + 1) {
      valves[dst] = true;
      // For the next turn the players' roles are reversed
      int candidate = part2(p2, dst, t2, t1 - t - 1, valves, paths, rates);
      valves[dst] = false; // backtrack
      score = std::max(score, candidate + (t1 - t - 1) * rates[dst]);
    }
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

  // Get the shortest paths in the form '[src, dst] -> length', then convert to
  // 'src -> [dst, length]', omitting zero-length paths and paths to or from
  // valves with rate 0, but keeping paths out of the start node.
  std::multimap<int, std::pair<int, int>> paths;
  for (auto [path, time]: get_shortest_paths(tunnels)) {
    auto [src, dst] = path;
    if (src != dst && (src == (int)names["AA"] || rates[src]) && rates[dst]) {
      paths.emplace(src, std::make_pair(dst, time));
    }
  }

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
