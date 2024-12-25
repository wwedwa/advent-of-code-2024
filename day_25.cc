#include "utils.h"

// Stores the lock and key heights
using Infoformat = std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>>;

Infoformat GetInfo() {
  Infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  // Process the heights for each key/lock
  while (std::getline(input, line)) {
    bool is_key = (line == ".....");
    std::vector<int> heights(5, 0);
    for (int i = 0; i < 5; ++i) {
      std::getline(input, line);
      for (int j = 0; j < line.size(); ++j) {
        heights[j] += (line[j] == '#');
      }
    }
    // Skip new line after each lock
    std::getline(input, line);
    std::getline(input, line);

    if (is_key) {
      info.second.push_back(heights);
    } else {
      info.first.push_back(heights);
    }
  }
  return info;
}

bool CheckLockAndKey(const std::vector<int>& lock, const std::vector<int>& key) {
  for (int i = 0; i < lock.size(); ++i) {
    if (lock[i] + key[i] > 5) {
      return false;
    }
  }
  return true;
}

int PartOne(const std::vector<std::vector<int>>& locks, const std::vector<std::vector<int>>& keys) {
  int answer = 0;
  for (const std::vector<int>& lock : locks) {
    for (const std::vector<int>& key : keys) {
      answer += CheckLockAndKey(lock, key);
    }
  }
  return answer;
}

int main() {
  auto start = std::chrono::steady_clock::now();
  Infoformat info = GetInfo();
  auto read_done = std::chrono::steady_clock::now();

  int answer_one = PartOne(info.first, info.second);
  auto part_one_done = std::chrono::steady_clock::now();

  std::cout << "Part One Answer: " << answer_one << std::endl;

  // Calculate run time of read, part one, and part two
  double read_time = std::chrono::duration_cast<
    std::chrono::duration<double>>(read_done - start).count();
  double part_one_time = std::chrono::duration_cast<
    std::chrono::duration<double>>(part_one_done - read_done).count();
  std::cout << "\n------------Time analysis------------\n"
            << "READ TIME: " << read_time << " seconds\n"
            << "PART ONE TIME: " << part_one_time << " seconds" << std::endl;

  return 0;
}
