#include "utils.h"

using Infoformat = std::list<long>;

Infoformat GetInfo() {
  Infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  while (std::getline(input, line)) {
    std::vector<long> num_vec = aoc::GetLongs(aoc::split(line, " "));
    // Copy vector into list for the insertion speed
    info = std::list<long>(num_vec.begin(), num_vec.end());
  }
  return info;
}

std::vector<long> Transform(long num) {
  // For easy digit counting and splitting
  std::string num_str = std::to_string(num);
  if (num == 0) {
    return {1};
  } else if (num_str.size() % 2 == 0) {
    return {std::stol(num_str.substr(0, num_str.size() / 2)),
            std::stol(num_str.substr(num_str.size() / 2))};
  } else {
    return {num * 2024};
  }
}

void Blink(Infoformat& info) {
  for (auto it = info.begin(); it != info.end(); ++it) {
    std::vector<long> new_nums = Transform(*it);
    if (new_nums.size() == 1) {
      *it = new_nums[0];
    } else {
      it = info.erase(it);
      info.insert(it, new_nums[0]);
      it = info.insert(it, new_nums[1]);
    }
  }
}

long DFSApproach(const Infoformat& info, int depth) {
  // Store value, depth, and id
  long answer = 0;

  auto GetNeighbors = [](const long& node) -> std::vector<long> {
    // If we are at max depth, no more neighbors
    return Transform(node);
  };

  auto ProcessPath = [&answer, &depth](const std::vector<long>& path) {
    if (path.size() == depth + 1) {
      ++answer;
    }
  };

  for (const long& start : info) {
    aoc::DFS<long>(start,
                  GetNeighbors,
                  ProcessPath,
                  [](const std::vector<long>&) { return false; },
                  true,  // revisit nodes
                  true,  // allow cycles
                  depth);  // max depth of cycle
  }
  return answer;
}

int PartOne(Infoformat info) {
  for (int i = 0; i < 25; ++i) {
    Blink(info);
  }
  return info.size();
}

// Create function for finding out what a 0 becomes after n steps
int PartTwo(Infoformat info) {
  int answer = 0;
  return answer;
}

int main() {
  auto start = std::chrono::steady_clock::now();
  Infoformat info = GetInfo();
  auto read_done = std::chrono::steady_clock::now();

  int answer_one = PartOne(info);
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
