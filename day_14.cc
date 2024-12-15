#include "utils.h"

using Coord = std::pair<int, int>;

const int kHeight = 103;
const int kWidth = 101;

struct Robot {
  Coord pos;
  Coord vel;

  void Step(int steps) {
    pos.first = aoc::PosMod(pos.first + steps * vel.first, kWidth);
    pos.second = aoc::PosMod(pos.second + steps * vel.second, kHeight);
  }
};

using Infoformat = std::vector<Robot>;

Infoformat GetInfo() {
  Infoformat info;
  std::ifstream input("input.txt");
  std::string pattern = "-?\\d+,-?\\d+";

  std::string line;
  while (std::getline(input, line)) {
    std::vector<std::string> p_v = aoc::FindAllRegexStrings(line, pattern);
    std::vector<int> p_vec = aoc::GetInts(aoc::split(p_v[0], ","));
    std::vector<int> v_vec = aoc::GetInts(aoc::split(p_v[1], ","));

    Robot robot;
    robot.pos = {p_vec[0], p_vec[1]};
    robot.vel = {v_vec[0], v_vec[1]};
    info.push_back(robot);
  }
  return info;
}

int PartOne(const Infoformat& info) {
  int q1_count = 0;
  int q2_count = 0;
  int q3_count = 0;
  int q4_count = 0;

  for (Robot robot : info) {
    robot.Step(100);
    if (robot.pos.first + 1 <= kWidth / 2 && robot.pos.second + 1 <= kHeight / 2) {
      ++q1_count;
    } else if (robot.pos.first + 1 > std::ceil(kWidth / 2.0) && robot.pos.second + 1 <= kHeight / 2) {
      ++q2_count;
    } else if (robot.pos.first + 1 <= kWidth / 2 && robot.pos.second + 1 > std::ceil(kHeight / 2.0)) {
      ++q3_count;
    } else if (robot.pos.first + 1 > std::ceil(kWidth / 2.0) && robot.pos.second + 1 > std::ceil(kHeight / 2.0)) {
      ++q4_count;
    }
  }

  return q1_count * q2_count * q3_count * q4_count;
}

void Print(const std::set<Coord> positions) {
  for (int i = 0; i < kHeight; ++i) {
    for (int j = 0; j < kWidth; ++j) {
      if (positions.count({j, i})) {
        std::cout << "#";
      } else {
        std::cout << ".";
      }
    }
    std::cout << std::endl;
  }
}

void StepRobots(Infoformat& info, int step_num) {
  for (Robot& robot : info) {
    robot.Step(step_num);
  }
}

// Found by printing first 300 iterations:
//      -- Horizontal lines appear at 4 seconds and repeat every 103 (4, 107, 210, etc.)
//      -- Vertical lines appear at 29 seconds and repeat every 101 (29, 130, 231, etc.)
//
// So solve y = 4 + 103a and y = 29 + 101b
// Equivalenty, y = 4 (mod 103), y = 29 (mod 101)
// 4 + 103a = 29 (mod 101)
// 103a = 25 (mod 101)
// 2a = 25 (mod 101) since 103 = 2 (mod 101)
// 2a = 126 (mod 101) since 25 = 126 (mod 101)
// a = 63 (mod 101) note we can divide by two since gcd(2, 101) = 1
// a = 101b + 63
// Plugging back into y, we get y = 4 + 103*(101b + 63) = 10403b + 6493
// Thus, horizontal and vertical lines appear after 6493 seconds and every 10403 seconds after
int PartTwo(Infoformat& info) {
  int answer = 6493;

  StepRobots(info, 6493);

  std::set<Coord> positions;
  for (Robot& robot : info) {
    positions.insert(robot.pos);
  }

  std::cout << "After " << answer << " seconds:" << std::endl;
  Print(positions);

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
