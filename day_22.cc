#include "utils.h"

using ullong = unsigned long long;
using Infoformat = std::vector<int>;

Infoformat GetInfo() {
  Infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  while (std::getline(input, line)) {
    info.push_back(std::stoi(line));
  }
  return info;
}

int GetNextNumber(int num) {
 // 16777216 is 2^24 so if we AND the result with 16777216-1, it is equivalent to using modulo
 num = ((num << 6) ^ num) & 16777215;
 num = ((num >> 5) ^ num) & 16777215;
 num = ((num << 11) ^ num) & 16777215;
 return num;
}

ullong PartOne(Infoformat& info) {
  ullong answer = 0;
  for (int num : info) {
    for (int i = 0; i < 2000; ++i) {
      num = GetNextNumber(num);
    }
    answer += num;
  }
  return answer;
}

void ShiftArray(std::array<int, 4>* arr, int new_elem) {
  for (int i = 0; i < 3; ++i) {
    (*arr)[i] = (*arr)[i + 1];
  }
  (*arr)[3] = new_elem;
}

int PartTwo(Infoformat& info) {
  int answer = 0;
  std::map<std::array<int, 4>, int> banana_count;
  std::array<int, 4> price_changes;

  for (int num : info) {
    int old_num = num;
    std::set<std::array<int, 4>> already_found;
    for (int i = 0; i < 2000; ++i) {
      num = GetNextNumber(num);
      int change = (num % 10) - (old_num % 10);
      old_num = num;
      if (i < 4) {
        // We need to get 4 changes before we do anything else
        price_changes[i] = change;
      } else {
        // If we already have 4 changes, shift array to include the new change
        ShiftArray(&price_changes, change);
      }
      if (i >= 3) {
        // Once we have 4 changes, start keeping track of them 
        if (!already_found.count(price_changes)) {
          already_found.insert(price_changes);
          banana_count[price_changes] += num % 10;
          if (answer < banana_count[price_changes]) {
            answer = banana_count[price_changes];
          }
        }
      }
    }
  }
  return answer;
}

int main() {
  auto start = std::chrono::steady_clock::now();
  Infoformat info = GetInfo();
  auto read_done = std::chrono::steady_clock::now();

  ullong answer_one = PartOne(info);
  auto part_one_done = std::chrono::steady_clock::now();

  int answer_two = PartTwo(info);
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
