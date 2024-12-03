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

  // Index keeping track of elemnt under inspection in the first column
  int j = 0;
  // Count to keep track of how many times col_one[j] appears in col_two
  int count = 0;
  // Lag for index over col_two. When j progresses, we do not want i to progress
  int lag = 0;

  for (int i = 0; i - lag < col_two.size() && j < col_one.size(); ++i) {
    if (col_two[i - lag] == col_one[j]) {
      ++count;
    } else if (col_two[i - lag] > col_one[j]) {
      answer += num_appearances[col_one[j]] * count * col_one[j];
      count = 0;
      ++lag;
      ++j;
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
