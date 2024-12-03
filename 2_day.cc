#include "utils.h"

typedef std::vector<std::vector<int>> infoformat;

infoformat GetInfo() {
  infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  while (std::getline(input, line)) {
    std::vector<int> num_strings = aoc::GetInts(aoc::split(line));
    info.push_back(num_strings);
  }
  return info;
}

bool IsSafe(std::vector<int> levels) {
  if (levels.empty()) {
    return true;
  }

  // Flags for keeping track if list is decreasing, increasing, or not known
  bool is_decreasing = false;
  bool unknown = true;
  // Track previous level
  int prev_level = levels[0];
  // Remove first level so that loop starts with the second
  levels.erase(levels.begin());

  for (int level : levels) {
    if (std::abs(prev_level - level) < 1 || std::abs(prev_level - level) > 3) {
      return false;
    }
    if (unknown && prev_level != level) {
      is_decreasing = (prev_level > level);
      unknown = false;
    } else if (!unknown && is_decreasing != (prev_level >= level)) {
      return false;
    }
    prev_level = level;
  }
  return true;
}

int PartOne(infoformat& info) {
  int answer = 0;
  for (std::vector<int>& levels : info) {
    answer += IsSafe(levels);
  }
  return answer;
}

int PartTwo(infoformat& info) {
  int answer = 0;
  // Just check if each level list is safe with each element removed.
  // Definitely not optimized but gets the job done.
  for (std::vector<int>& levels : info) {
    for (int i = 0; i < levels.size(); ++i) {
      std::vector<int> copy_levels = levels;
      copy_levels.erase(copy_levels.begin() + i);
      bool is_safe = IsSafe(copy_levels);
      if (is_safe) {
        ++answer;
        break;
      }
    }
  }
  return answer;
}

int main() {
  infoformat info = GetInfo();
  int answer_one = PartOne(info);
  int answer_two = PartTwo(info);
  std::cout << "Part One: " << answer_one << std::endl;
  std::cout << "Part Two: " << answer_two << std::endl;
  return 0;
}
