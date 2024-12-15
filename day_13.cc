#include "utils.h"

using llong = long long;
using ullong = unsigned long long;
using Coord = std::pair<llong, llong>;
// Contains three Coords: First one is the x,y movement for button A, 
// second one is x,y movement for button B, third one is prize location
using Infoformat = std::vector<std::tuple<Coord, Coord, Coord>>;

Infoformat GetInfo() {
  Infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  
  while (std::getline(input, line)) {
    if (line.empty()) {
      continue;  // skip new lines
    }
    std::vector<std::string> a_button = aoc::FindAllRegexStrings(line, "\\d+");
    Coord a_movement = {std::stoll(a_button[0]), std::stoll(a_button[1])};
    std::getline(input, line);

    std::vector<std::string> b_button = aoc::FindAllRegexStrings(line, "\\d+");
    Coord b_movement = {std::stoll(b_button[0]), std::stoll(b_button[1])};
    std::getline(input, line);

    std::vector<std::string> prize = aoc::FindAllRegexStrings(line, "\\d+");
    Coord prize_loc = {std::stoll(prize[0]), std::stoll(prize[1])};

    info.emplace_back(a_movement, b_movement, prize_loc);
  }
  return info;
}

// Each machine can be represented as two linear equations:
//     -- ax + by = c
//     -- dx + ey = f
// where:
//     a and d are the x,y movements corresponding to the A button
//     b and e are the x,y movements corresponding to the B button
//     c and f are the x,y coords corresponding to the prize location
//     x and y are the number of times the A and B buttons are pressed
Coord FindSol(Coord a_move, Coord b_move, Coord prize_loc) {
  llong a = a_move.first;
  llong d = a_move.second;
  llong b = b_move.first;
  llong e = b_move.second;
  llong c = prize_loc.first;
  llong f = prize_loc.second;

  llong determinant = b * d - a * e;
  // no sol, indicate with (-1, -1)
  if (determinant == 0) {
    return {-1, -1};
  }
  double y = (c * d - f * a) / (double)determinant;
  double intpart = 0;
  // Only want positive integer solutions
  if (modf(y, &intpart) != 0.0 || y < 0) {
    return {-1, -1};
  }
  double x = ((c * d) - (b * d * y)) / ((double)a * d);
  // Again, only want positive integer solutions
  if (modf(x, &intpart) != 0.0 || x < 0) {
    return {-1, -1};
  }

  return {(llong)x, (llong)y};
}

llong PartOne(const Infoformat& info) {
  llong answer = 0;

  Coord a_move;
  Coord b_move;
  Coord prize_loc;

  int count = 1;
  for (const std::tuple<Coord, Coord, Coord>& machine : info) {
    a_move = std::get<0>(machine);
    b_move = std::get<1>(machine);
    prize_loc = std::get<2>(machine);
    Coord sol = FindSol(a_move, b_move, prize_loc);
    //std::cout << "Sol for machine " << count << ": (" << sol.first << ", " << sol.second << ")" << std::endl;
    ++count;
    if (sol.first == -1 && sol.second == -1) {
      continue;
    }
    answer += 3 * sol.first + sol.second;
    std::cout << "Looking at machine " << count << std::endl;
  }
  return answer;
}

// Exact same as part one except add the offset
llong PartTwo(const Infoformat& info) {
  llong answer = 0;

  Coord a_move;
  Coord b_move;
  Coord prize_loc;

  llong offset = 10000000000000;

  int count = 1;
  for (const std::tuple<Coord, Coord, Coord>& machine : info) {
    a_move = std::get<0>(machine);
    b_move = std::get<1>(machine);
    prize_loc = std::get<2>(machine);
    Coord new_prize_loc = {prize_loc.first + offset, prize_loc.second + offset};
    Coord sol = FindSol(a_move, b_move, new_prize_loc);
    //std::cout << "Sol for machine " << count << ": (" << sol.first << ", " << sol.second << ")" << std::endl;
    ++count;
    if (sol.first == -1 && sol.second == -1) {
      continue;
    }
    answer += 3 * sol.first + sol.second;
    std::cout << "Looking at machine " << count << std::endl;
  }
  return answer;
}

int main() {
  auto start = std::chrono::steady_clock::now();
  Infoformat info = GetInfo();
  auto read_done = std::chrono::steady_clock::now();

  llong answer_one = PartOne(info);
  auto part_one_done = std::chrono::steady_clock::now();

  llong answer_two = PartTwo(info);
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
