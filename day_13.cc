#include "utils.h"

using Coord = std::pair<int, int>;
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
    Coord a_movement = {std::stoi(a_button[0]), std::stoi(a_button[1])};
    std::getline(input, line);

    std::vector<std::string> b_button = aoc::FindAllRegexStrings(line, "\\d+");
    Coord b_movement = {std::stoi(b_button[0]), std::stoi(b_button[1])};
    std::getline(input, line);

    std::vector<std::string> prize = aoc::FindAllRegexStrings(line, "\\d+");
    Coord prize_loc = {std::stoi(prize[0]), std::stoi(prize[1])};

    info.emplace_back(a_movement, b_movement, prize_loc);
  }
  return info;
}

// Movement in these machines can be represented as a linear Diophantine
// equation ax + by = c. These only have sols when gcd(a,b) | c
// Thanks number theory class for this quick check
// Note that this returns true even if there is a non positive solution
// To check for only positive we would need to use extended Euclidean algorithm
// but I'm trusting this is sufficent for now.
bool IsPossible(Coord a_move, Coord b_move, Coord prize_loc) {
  // Check if movement is possible in each axis
  bool is_x_possible = prize_loc.first % std::gcd(a_move.first, b_move.first) == 0;
  bool is_y_possible = prize_loc.second % std::gcd(a_move.second, b_move.second) == 0;
  return is_x_possible && is_y_possible;
}

// No more than 100 presses for each button
std::pair<bool, bool> ValidPressCount(const Coord& node, const std::map<Coord, Coord>& prev_nodes, const Coord& a_move) {
  int a_count = 0;
  int b_count = 0;
  bool path_done = false;
  Coord curr_node = node;
  while (!path_done) {
    Coord new_node = prev_nodes.at(curr_node);
    if (new_node == curr_node) {
      path_done = true;
    }
    if (new_node.first - curr_node.first == a_move.first && new_node.second - curr_node.second == a_move.second) {
      ++a_count;
    } else {
      ++b_count;
    }
    curr_node = new_node;
  }
  return {a_count < 100, b_count < 100};
}



int PartOne(const Infoformat& info) {
  int answer = 0;

  Coord start = {0, 0};
  Coord a_move;
  Coord b_move;
  Coord prize_loc;

  auto GetNeighbors = [&](const Coord& node,
                        const std::map<Coord, Coord>& prev_nodes,
                        const std::map<Coord, int>& dists) -> 
                        std::vector<std::pair<int, Coord>> {
    std::vector<std::pair<int, Coord>> neighbors;
    auto [add_a, add_b] = ValidPressCount(node, prev_nodes, a_move);
    Coord a_neighbor = {node.first + a_move.first, node.second + a_move.second};
    Coord b_neighbor = {node.first + b_move.first, node.second + b_move.second};

    if (add_a && a_neighbor.first <= prize_loc.first && a_neighbor.second <= prize_loc.second) {
      neighbors.emplace_back(3, a_neighbor);
    }
    if (add_b && b_neighbor.first <= prize_loc.first && b_neighbor.second <= prize_loc.second) {
      neighbors.emplace_back(1, b_neighbor);
    }
    return neighbors;
  };

  auto ProcessPath = [](const Coord&, const std::map<Coord, Coord>&, const std::map<Coord, int>&) {};

  auto EndCondition = [&](const Coord& node,
                        const std::map<Coord, Coord>& prev_nodes,
                        const std::map<Coord, int>& dists) -> 
                        bool {
    if (node == prize_loc) {
      answer += dists.at(node);
      return true;
    }
    return false;
  };

  int count = 1;
  for (const std::tuple<Coord, Coord, Coord>& machine : info) {
    a_move = std::get<0>(machine);
    b_move = std::get<1>(machine);
    prize_loc = std::get<2>(machine);
    if (!IsPossible(a_move, b_move, prize_loc)) {
      continue;
    }
    std::cout << "Looking at machine " << count << std::endl;
    ++count;

    aoc::Dijkstra<Coord>(start, GetNeighbors, ProcessPath, EndCondition);
  }
  return answer;
}

int PartTwo(const Infoformat& info) {
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
