#include "utils.h"

typedef std::vector<std::vector<int>> infoformat;

infoformat GetInfo() {
  infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  while (std::getline(input, line)) {

  }
  return info;
}

int PartOne(infoformat& info) {
  int answer = 0;
  return answer;
}

int PartTwo(infoformat& info) {
  int answer = 0;
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
