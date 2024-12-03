#include "utils.h"

typedef std::pair<std::vector<int>, std::vector<int>> infoformat;

infoformat GetInfo() {
  infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  while (std::getline(input, line)) {
    std::vector<std::string> num_strings = aoc::split(line);
    std::vector<int> nums = aoc::GetInts(num_strings);
    info.first.push_back(nums[0]);
    info.second.push_back(nums[1]);
  }
  return info;
}

int PartOne(std::vector<int>& col_one, std::vector<int>& col_two) {
  int answer = 0;
  std::sort(col_one.begin(), col_one.end());
  std::sort(col_two.begin(), col_two.end());

  for (int i = 0; i < col_one.size(); ++i) {
    int difference = col_one[i] - col_two[i];
    if (difference >= 0) {
      answer += difference;
    } else {
      answer -= difference;
    }
  }
  return answer;
}

int PartTwo(std::vector<int>& col_one, std::vector<int>& col_two) {
  int answer = 0;
  std::sort(col_one.begin(), col_one.end());
  std::sort(col_two.begin(), col_two.end());

  // Count how many times each element appears in the first column
  std::map<int, int> num_appearances;
  for (int num : col_one) {
    num_appearances[num] += 1;
  }

  // Remove duplicates from first column
  auto it = std::unique(col_one.begin(), col_one.end());
  col_one.erase(it, col_one.end());

  // Index keeping track of element under inspection in the second column
  int j = 0;
  // Count to keep track of how many times elem in col_one appears in col_two
  int count = 0;

  for (int i = 0; i < col_one.size() && j < col_two.size(); ++i) {
    while (col_one[i] >= col_two[j] && j < col_two.size()) {
      // 1 if the elements in the columns are equal. 0 otherwise
      count += (col_one[i] == col_two[j]);
      ++j;
    }
    if (col_one[i] < col_two[j]) {
      answer += num_appearances[col_one[i]] * count * col_one[i];
      count = 0;
    }
  }
  return answer;
}

int main() {
  infoformat info = GetInfo();
  int answer_one = PartOne(info.first, info.second);
  int answer_two = PartTwo(info.first, info.second);
  std::cout << "Part One: " << answer_one << std::endl;
  std::cout << "Part Two: " << answer_two << std::endl;
  return 0;
}
