#include "utils.h"

using ullong = unsigned long long;
// Holds the patterns we can build with and the patterns we need to build
using Infoformat = std::pair<std::vector<std::string>, std::vector<std::string>>;

Infoformat GetInfo() {
  Infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  std::getline(input, line);
  info.first = aoc::split(line, ", ");
  std::getline(input, line);  // Move past new line
  while (std::getline(input, line)) {
    info.second.push_back(line);
  }
  return info;
}

// Cache if a certain string can be made or not
std::unordered_map<std::string, bool> made_cache;

bool CanBeMade(std::string pattern, const std::vector<std::string>& towels) {
  // If pattern has been completed, increment count and return
  if (pattern.empty()) {
    return true;
  }
  // If this pattern has been cached, return the previously found result
  if (made_cache.count(pattern)) {
    return made_cache[pattern];
  }
  for (std::string towel : towels) {
    // If the towel can be used to build the pattern, check if the rest of the
    // pattern can be built. If so, return true
    if (pattern.substr(0, towel.size()) == towel &&
        CanBeMade(pattern.substr(towel.size()), towels)) {
      made_cache[pattern] = true;
      return true;
    }
  }
  // When all towels have been checked and the pattern cannot be matched,
  // return false
  made_cache[pattern] = false;
  return false;
}

// Cache to count the number of ways a pattern can be made
std::unordered_map<std::string, ullong> count_cache;

void NumOfWays(std::string pattern, std::vector<std::string> towels, ullong& count) {
  ullong saved_count = count;
  // If this pattern cannot be made, return
  if (made_cache.count(pattern) && !made_cache[pattern]) {
    count_cache[pattern] = 0;
    return;
  }
  // If pattern has been completed, increment count and return
  if (pattern.empty()) {
    ++count;
    return;
  }
  // If this pattern has been fully explored, add its cached count and return
  if (count_cache.count(pattern)) {
    count += count_cache[pattern];
    return;
  }
  for (std::string towel : towels) {
    // If the towel can be used to build the pattern, check if the rest of the
    // pattern can be built. If so, return true
    if (pattern.substr(0, towel.size()) == towel) {
      NumOfWays(pattern.substr(towel.size()), towels, count);
    }
  }
  // Figure out how many ways the sub-pattern can be made
  count_cache[pattern] = count - saved_count;
  // If count has changed, the pattern can be made, else it cannot
  made_cache[pattern] = (count != saved_count);
  return;
}

int PartOne(const std::vector<std::string>& towels, const std::vector<std::string>& patterns) {
  int answer = 0;
  for (const std::string& pattern : patterns) {
    answer += CanBeMade(pattern, towels);
  }
  return answer;
}

ullong PartTwo(const std::vector<std::string>& towels, const std::vector<std::string>& patterns) {
  ullong answer = 0;
  for (const std::string& pattern : patterns) {
    if (!made_cache[pattern]) {
      continue;  // skip if pattern cannot be made
    }
    ullong count = 0;
    NumOfWays(pattern, towels, count);
    answer += count;
  }
  return answer;
}

int main() {
  auto start = std::chrono::steady_clock::now();
  Infoformat info = GetInfo();
  auto read_done = std::chrono::steady_clock::now();

  int answer_one = PartOne(info.first, info.second);
  auto part_one_done = std::chrono::steady_clock::now();

  ullong answer_two = PartTwo(info.first, info.second);
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
