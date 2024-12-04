#include "utils.h"

std::string GetInfo() {
  std::string info;
  std::ifstream input("input.txt");
  std::string line;
  while (std::getline(input, line)) {
    info += line;
  }
  return info;
}

int PerformMult(std::string mult) {
  // Remove "mul(" and ")" from beginning and end
  mult = mult.substr(4);
  mult = mult.substr(0, mult.size() - 1);
  
  std::vector<int> nums = aoc::GetInts(aoc::split(mult, ","));
  return nums[0] * nums[1];
}

int PartOne(std::string& info) {
  int answer = 0;
  std::string pattern = "mul\\(([1-9][0-9]{0,2}),([1-9][0-9]{0,2})\\)";
  std::vector<std::string> occurences = aoc::MatchRegex(info, pattern);
  for (std::string mult : occurences) {
    answer += PerformMult(mult);
  }
  return answer;
}

int PartTwo(std::string& info) {
  int answer = 0;
  std::string pattern = "mul\\(([1-9][0-9]{0,2}),([1-9][0-9]{0,2})\\)|do\\(\\)|don't\\(\\)";
  std::vector<std::string> occurences = aoc::MatchRegex(info, pattern);

  // Keep track of if we should add the mults or not
  bool enabled = true;
  
  for (std::string command : occurences) {
    if (command == "do()") {
      enabled = true;
    } else if (command == "don't()") {
      enabled = false;
    } else if (enabled) {
      answer += PerformMult(command);
    }
  }
  return answer;
}

int main() {
  std::string info = GetInfo();
  int answer_one = PartOne(info);
  int answer_two = PartTwo(info);
  std::cout << "Part One: " << answer_one << std::endl;
  std::cout << "Part Two: " << answer_two << std::endl;
  return 0;
}
