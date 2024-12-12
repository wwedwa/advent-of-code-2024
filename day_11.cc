#include "utils.h"

using ullong = unsigned long long;
using Infoformat = std::unordered_map<ullong, ullong>;

Infoformat GetInfo() {
  Infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  while (std::getline(input, line)) {
    std::vector<ullong> num_vec = aoc::GetULLongs(aoc::split(line, " "));
    // Make map
    for (ullong stone : num_vec) {
      info[stone] += 1;
    }
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

Infoformat Blink(const Infoformat& cache) {
  Infoformat new_cache;
  for (auto [stone, count] : cache) {
    for (ullong new_stone : Transform(stone)) {
      new_cache[new_stone] += count;
    }
  }
  return new_cache;
}

int PartOne(Infoformat info) {
  for (int i = 0; i < 25; ++i) {
    info = Blink(info);
  }
  int answer = 0;
  for (auto [_, count] : info) {
    answer += count;
  }
  return answer;
}


// This caching solution relies on the fact that at any given time, there aren't
// that many distinct stones. So we can actually just group them together.
// Also, order of the stones does not matter (even though the problem would
// lead you to believe it does)
ullong PartTwo(Infoformat info) {
  for (int i = 0; i < 75; ++i) {
    info = Blink(info);
  }
  ullong answer = 0;
  for (auto [_, count] : info) {
    answer += count;
  }
  return answer;
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
