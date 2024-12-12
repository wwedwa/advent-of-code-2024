#include "utils.h"

using ullong = unsigned long long;
using Infoformat = std::list<ullong>;

Infoformat GetInfo() {
  Infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  while (std::getline(input, line)) {
    std::vector<ullong> num_vec = aoc::GetULLongs(aoc::split(line, " "));
    // Copy vector into list for the insertion speed
    info = std::list<ullong>(num_vec.begin(), num_vec.end());
  }
  return info;
}

std::vector<ullong> Transform(ullong num) {
  // For easy digit counting and splitting
  std::string num_str = std::to_string(num);
  if (num == 0) {
    return {1};
  } else if (num_str.size() % 2 == 0) {
    return {std::stoull(num_str.substr(0, num_str.size() / 2)),
            std::stoull(num_str.substr(num_str.size() / 2))};
  } else {
    return {num * 2024};
  }
}

void Blink(Infoformat& info) {
  for (auto it = info.begin(); it != info.end(); ++it) {
    std::vector<ullong> new_nums = Transform(*it);
    if (new_nums.size() == 1) {
      *it = new_nums[0];
    } else {
      it = info.erase(it);
      info.insert(it, new_nums[0]);
      it = info.insert(it, new_nums[1]);
    }
  }
}

ullong DFSApproach(const Infoformat& info, int depth) {
  ullong answer = 0;

  // Key is stone value. Value is a map where key is n and value
  // is a pair containing resulting number of stones after n blinks and
  // if this value is final
  std::map<ullong, std::map<int, std::pair<ullong, bool>>> cache;

  auto GetNeighbors = [](const ullong& node) -> std::vector<ullong> {
    // If we are at max depth, no more neighbors
    return Transform(node);
  };

  auto PreProcessPath = [&answer, &depth, &cache](const std::vector<ullong>& path) {
    if (cache[path.back()][0].second) {
      return;
    }
    if (path.size() != depth + 1) {
      return;
    }
    for (int i = 0; i < path.size(); ++i) {
      if (!cache.count(path[i])) {
        cache[path[i]][depth - i] = {0, false};
      }
      ++cache[path[i]][depth - i].first;
    }
    ++answer;
  };

  auto PostProcessPath = [&depth, &cache](const std::vector<ullong>& path) {
    for (int i = 0; i < path.size(); ++i) {
      if (cache.count(path[i])) {
        cache[path[i]][depth - i].second = true;
      }
    }
  };

  auto EndCondition = [&answer, &depth, &cache](const std::vector<ullong>& path) -> bool {
    if (cache.count(path.back())) {
      std::pair<ullong, bool> cache_entry = cache[path.back()][depth - path.size() + 1];
      if (cache_entry.second) {
        answer += cache_entry.first;
        for (int i = 0; i < path.size() - 1; ++i) {
          if (cache.count(path[i])) {
            cache[path[i]][depth - i].first += cache_entry.first;
          }
        }
        return true;
      }
    }
    return false;
  };

  for (const ullong& start : info) {
    aoc::DFS<ullong>(start,
                  GetNeighbors,
                  PreProcessPath,
                  PostProcessPath,
                  EndCondition,
                  true,  // revisit nodes
                  true,  // allow cycles
                  depth);  // max depth of cycle
  }
  return answer;
}

int PartOne(Infoformat info) {
  for (int i = 0; i < 25; ++i) {
    Blink(info);
  }
  return info.size();
}

// Create function for finding out what a 0 becomes after n steps
ullong PartTwo(Infoformat info) {
  return DFSApproach(info, 75);
}

int main() {
  auto start = std::chrono::steady_clock::now();
  Infoformat info = GetInfo();
  auto read_done = std::chrono::steady_clock::now();

  int answer_one = PartOne(info);
  auto part_one_done = std::chrono::steady_clock::now();

  ullong answer_two = PartTwo(info);
  auto part_two_done = std::chrono::steady_clock::now();

  std::cout << "Part One Answer: " << answer_one << std::endl;
  std::cout << "Part Two Answer: " << answer_two << std::endl;

  // Calculate run time of read, part one, and part two
  double read_time = std::chrono::duration_cast<
    std::chrono::duration<double>>(read_done - start).count();
  double part_one_time = std::chrono::duration_cast<
    std::chrono::duration<double>>(part_one_done - read_done).count();
  double part_two_time = std::chrono::duration_cast<
    std::chrono::duration<double>>(part_two_done - part_one_done).count();
  std::cout << "\n------------Time analysis------------\n"
            << "READ TIME: " << read_time << " seconds\n"
            << "PART ONE TIME: " << part_one_time << " seconds\n"
            << "PART TWO TIME: " << part_two_time << " seconds" << std::endl;

  return 0;
}
