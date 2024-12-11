#include "utils.h"

using Coord = std::pair<int, int>;
// This holds the starting locations and map
using Infoformat = std::pair<std::vector<Coord>, std::vector<std::vector<char>>>;

Infoformat GetInfo() {
  Infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  int i = 0;
  while (std::getline(input, line)) {
    std::vector<char> char_line;
    for (int j = 0; j < line.size(); ++j) {
      char_line.push_back(line[j]);
      if (line[j] == '0') {
        info.first.push_back({i, j});
      }
    }
    info.second.push_back(char_line);
    ++i;
  }
  return info;
}

bool CheckIfOnMap(int length, int height, const Coord& node) {
  return (node.first >= 0 &&
          node.first < height &&
          node.second >= 0 &&
          node.second < length);
}

std::vector<Coord> GetNodeNeighbors(const Coord& node, const std::vector<std::vector<char>>& map) {
  std::vector<Coord> neighbors;
  
  int length = map[0].size();
  int height = map.size();

  // Check four surrounding neighbors
  if (CheckIfOnMap(length, height, {node.first + 1, node.second}) &&
      map[node.first + 1][node.second] - map[node.first][node.second] == 1) {
    neighbors.push_back({node.first + 1, node.second});
  }
  if (CheckIfOnMap(length, height, {node.first - 1, node.second}) &&
      map[node.first - 1][node.second] - map[node.first][node.second] == 1) {
    neighbors.push_back({node.first - 1, node.second});
  }
  if (CheckIfOnMap(length, height, {node.first, node.second + 1}) &&
      map[node.first][node.second + 1] - map[node.first][node.second] == 1) {
    neighbors.push_back({node.first, node.second + 1});
  }
  if (CheckIfOnMap(length, height, {node.first, node.second - 1}) &&
      map[node.first][node.second - 1] - map[node.first][node.second] == 1) {
    neighbors.push_back({node.first, node.second - 1});
  }

  return neighbors;
}

int PartOne(const std::vector<Coord>& starts, const std::vector<std::vector<char>>& map) {
  int answer = 0;

  auto GetNeighbors = [&map](const Coord& node) -> std::vector<Coord> {
    return GetNodeNeighbors(node, map);
  };

  auto ProcessPath = [&answer](const std::vector<Coord>& path) {
    // Based on problem's construction, path from 0 to 9 will always have length 10
    if (path.size() == 10) {
      ++answer;
    }
  };

  for (const Coord& start : starts) {
    aoc::BFS<Coord>(start,
                    GetNeighbors,
                    ProcessPath);
  }
  return answer;
}

int PartTwo(const std::vector<Coord>& starts, const std::vector<std::vector<char>>& map) {
  int answer = 0;

  auto GetNeighbors = [&map](const Coord& node) -> std::vector<Coord> {
    return GetNodeNeighbors(node, map);
  };

  auto ProcessPath = [&answer](const std::vector<Coord>& path) {
    // Based on problem's construction, path from 0 to 9 will always have length 10
    if (path.size() == 10) {
      ++answer;
    }
  };

  for (const Coord& start : starts) {
    aoc::BFS<Coord>(start,
                    GetNeighbors,
                    ProcessPath,
                    [](const std::vector<Coord>&) { return false; },
                    true);
  }

  return answer;
}

int main() {
  auto start = std::chrono::steady_clock::now();
  Infoformat info = GetInfo();
  auto read_done = std::chrono::steady_clock::now();

  int answer_one = PartOne(info.first, info.second);
  auto part_one_done = std::chrono::steady_clock::now();

  int answer_two = PartTwo(info.first, info.second);
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
