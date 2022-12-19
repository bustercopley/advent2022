#include "precompiled.hpp"

auto regex =
  re::regex(R"(^Blueprint (\d+): )"
            R"(Each ore robot costs (\d+) ore. )"
            R"(Each clay robot costs (\d+) ore. )"
            R"(Each obsidian robot costs (\d+) ore and (\d+) clay. )"
            R"(Each geode robot costs (\d+) ore and (\d+) obsidian.$)");

int get_score(const auto &cost, int time, auto &robots, auto &materials,
  int &max_score, auto &log) {

  int score{};
  if (time == 0) {
    score = materials[3];
  } else {

    // Prune 1: can't beat best score even if we build a geode robot every day
    if (materials[3] + time * robots[3] + time * (time + 1) / 2 < max_score) {
      return 0;
    }

    // What robots can we afford to build today?
    for (int i = 0; i != 5; ++i) {
      log[time][i] = cost[i][0] <= materials[0] && cost[i][1] <= materials[1] &&
        cost[i][2] <= materials[2] && cost[i][3] <= materials[3];
    }

    for (int i = 0; i != 5; ++i) {

      // Prune 2: could have built this robot yesterday but idled instead
      if (log[time + 1][i] == 1 && log[time + 1][4] == -1) {
        continue;
      }

      // What is our best score if we build this robot today?
      if (log[time][i]) {
        materials[0] += robots[0] - cost[i][0];
        materials[1] += robots[1] - cost[i][1];
        materials[2] += robots[2] - cost[i][2];
        materials[3] += robots[3] - cost[i][3];
        ++robots[i];
        log[time][i] = -1;

        score = std::max(score,
          get_score(cost, time - 1, robots, materials, max_score, log));

        // backtrack
        log[time][i] = 1;
        --robots[i];
        materials[3] -= robots[3] - cost[i][3];
        materials[2] -= robots[2] - cost[i][2];
        materials[1] -= robots[1] - cost[i][1];
        materials[0] -= robots[0] - cost[i][0];
      }
    }
  }
  max_score = std::max(score, max_score);
  return score;
}

void solve(std::istream &stream) {
  std::int64_t result1{};
  std::int64_t result2{1};
  for (std::string line; std::getline(stream, line);) {
    if (auto m = match(regex, line)) {
      int cost[5][4]{};
      int index = string_to<int>(match_view(m, 1, line));
      cost[0][0] = string_to<int>(match_view(m, 2, line));
      cost[1][0] = string_to<int>(match_view(m, 3, line));
      cost[2][0] = string_to<int>(match_view(m, 4, line));
      cost[2][1] = string_to<int>(match_view(m, 5, line));
      cost[3][0] = string_to<int>(match_view(m, 6, line));
      cost[3][2] = string_to<int>(match_view(m, 7, line));
      std::array<int, 5> robots{1, 0, 0, 0, 0};
      std::array<int, 4> materials{0, 0, 0, 0};
      int max_score{};
      {
        int log[33][5]{};
        int score24 = get_score(cost, 24, robots, materials, max_score, log);
        result1 += index * score24;
      }
      if (index <= 3) {
        int log[33][5]{};
        int score32 = get_score(cost, 32, robots, materials, max_score, log);
        result2 *= score32;
      }
    }
  }
  std::printf("Part 1 result %lld\n"
              "Part 2 result %lld\n",
    result1, result2);
}

int main() {
  constexpr int day = 19;
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
