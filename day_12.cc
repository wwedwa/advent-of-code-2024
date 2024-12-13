#include "utils.h"

using Coord = std::pair<int, int>;
using Infoformat = std::vector<std::vector<char>>;

Infoformat GetInfo() {
  Infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  while (std::getline(input, line)) {
    std::vector<char> vector_line;
    for (char c : line) {
      vector_line.push_back(c);
    }
    info.push_back(vector_line);
  }
  return info;
}

enum class FenceType {
  kTop,
  kBottom,
  kRight,
  kLeft
};

FenceType GetFenceType(const Coord& dir) {
  if (dir.first == 1) return FenceType::kBottom;
  else if (dir.first == -1) return FenceType::kTop;
  else if (dir.second == 1) return FenceType::kRight;
  else return FenceType::kLeft;
}

bool IsOnMap(int length, int height, const Coord& node) {
  return node.first >= 0 && node.first < height &&
        node.second >= 0 && node.second < length;
}

int CountCorners(const Infoformat& info, const Coord& node) {
  int corner_num = 0;
  int length = info[0].size();
  int height = info.size();
  Coord dir = {-1, -1};
  char plot_type = info[node.first][node.second];
  for (int i = 0; i < 4; ++i) {
    Coord neighbor1 = {node.first + dir.first, node.second};
    Coord neighbor2 = {node.first, node.second + dir.second};
    Coord diag_neighbor = {node.first + dir.first, node.second + dir.second};
    bool is_convex = (!IsOnMap(length, height, neighbor1) ||
                      info[neighbor1.first][neighbor1.second] != plot_type) &&
                      (!IsOnMap(length, height, neighbor2) ||
                      info[neighbor2.first][neighbor2.second] != plot_type);

    bool is_concave = (IsOnMap(length, height, neighbor1) &&
                      info[neighbor1.first][neighbor1.second] == plot_type) &&
                      (IsOnMap(length, height, neighbor2) && 
                      info[neighbor2.first][neighbor2.second] == plot_type) && 
                      (IsOnMap(length, height, diag_neighbor) && 
                      info[diag_neighbor.first][diag_neighbor.second] != plot_type);

    corner_num += is_convex;
    corner_num += is_concave;

    // Turn direction
    int new_dir_first = -dir.second;
    dir.second = dir.first;
    dir.first = new_dir_first;
  }
  return corner_num;
}

std::vector<Coord> GetNeighborPlots(const Infoformat& info, const Coord& node) {
  std::vector<Coord> neighbors;
  int length = info[0].size();
  int height = info.size();

  // Starting direction we look in for a neighbor
  Coord dir = {0, 1};
  for (int i = 0; i < 4; ++i) {
    Coord neighbor = {node.first + dir.first, node.second + dir.second};

    // If nieghbor is the same character, add to neighbor vector
    if (IsOnMap(length, height, neighbor) && info[neighbor.first][neighbor.second] == info[node.first][node.second]) {
      neighbors.push_back(neighbor);  // When neighbor is part of group
    }

    // Turn direction
    int new_dir_first = -dir.second;
    dir.second = dir.first;
    dir.first = new_dir_first;
  }
  return neighbors;
}



// For part 2 966844 is too high
// Flood fill algorithm implemented with BFS
int PartOne(const Infoformat& info) {
  int answer = 0;

  int curr_perimeter = 0;
  int curr_area = 0;
  std::set<Coord> visited_nodes;

  auto GetNeighbors = [&](const Coord& node) -> std::vector<Coord> {
    std::vector<Coord> neighbors = GetNeighborPlots(info, node);
    curr_perimeter += 4 - neighbors.size();
    return neighbors;
  };

  // Every time we visit a new node, increment area. Technically, we could do
  // this in GetNeighbors since we do not care about the path at all in this
  // problem, but we do it here for separation of concerns
  auto ProcessPath = [&](const std::vector<Coord>& path) {
    visited_nodes.insert(path.back());
    ++curr_area;
  };

  int length = info[0].size();
  int height = info.size();
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < length; ++j) {
      if (visited_nodes.count({i, j})) {
        continue;
      }
      aoc::BFS<Coord>({i, j}, GetNeighbors, ProcessPath);
      answer += curr_area * curr_perimeter;
      // Reset area and perimeter counter
      curr_area = 0;
      curr_perimeter = 0;
    }
  }
  return answer;
}

// Same as part two except we count sides instead of perimeter in GetNeighbors
int PartTwo(Infoformat& info) {
  int answer = 0;

  int curr_sides = 0;
  int curr_area = 0;
  std::set<Coord> visited_nodes;

  auto GetNeighbors = [&](const Coord& node) -> std::vector<Coord> {
    std::vector<Coord> neighbors = GetNeighborPlots(info, node);
    // Using fact that polygons have same number of sides as they do corners.
    curr_sides += CountCorners(info, node);
    return neighbors;
  };

  // Every time we visit a new node, increment area. Technically, we could do
  // this in GetNeighbors since we do not care about the path at all in this
  // problem, but we do it here for separation of concerns
  auto ProcessPath = [&](const std::vector<Coord>& path) {
    visited_nodes.insert(path.back());
    ++curr_area;
  };

  int length = info[0].size();
  int height = info.size();
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < length; ++j) {
      if (visited_nodes.count({i, j})) {
        continue;
      }
      aoc::BFS<Coord>({i, j}, GetNeighbors, ProcessPath);
      answer += curr_area * curr_sides;
      // Reset area and sides counter
      curr_area = 0;
      curr_sides = 0;
    }
  }
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
