#include "utils.h"

int GetInfo() {
  std::ifstream input("input.txt");
  std::string line;
  while (std::getline(input, line)) {

  }
  return 0;
}

int PartOne() {
  int answer = 0;
  return answer;
}

int PartTwo() {
  int answer = 0;
  return answer;
}

int main() {
  int info = GetInfo();
  int answer_one = PartOne();
  int answer_two = PartTwo();
  std::cout << "Part One: " << answer_one << std::endl;
  std::cout << "Part Two: " << answer_two << std::endl;
  return 0;
}
