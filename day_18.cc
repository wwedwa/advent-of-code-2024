#include "utils.h"

using Coord = std::pair<int, int>;
using Infoformat = std::vector<Coord>;

int kMaxX = 70;
int kMaxY = 70;

Infoformat GetInfo() {
  Infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  while (std::getline(input, line)) {
    std::vector<int> coords = aoc::GetInts(aoc::split(line, ","));
    info.emplace_back(coords[0], coords[1]);
  }
  return info;
}

bool IsOnMap(Coord coord) {
  return coord.first >= 0 &&
        coord.first <= kMaxX &&
        coord.second >= 0 &&
        coord.second <= kMaxY;
}

int PartOne(Infoformat& info) {
  int answer = 0;
  Coord start = {0, 0};
  Coord end = {kMaxX, kMaxY};

  std::set<Coord> corrupted_mem;
  for (int i = 0; i < 1024; ++i) {
    corrupted_mem.insert(info[i]);
  }

  auto GetNeighbors = [&](const std::vector<Coord>& path) -> std::vector<Coord> {
    std::vector<Coord> neighbors;

    Coord curr_node = path.back();
    Coord dir = {0, 1};
    for (int i = 0; i < 4; ++i) {
      Coord poss_neighbor = {curr_node.first + dir.first, curr_node.second + dir.second};
      if (IsOnMap(poss_neighbor) && !corrupted_mem.count(poss_neighbor)) {
        neighbors.push_back(poss_neighbor);
      }
      // Rotate dir 90 deg
      int temp_dir = dir.first;
      dir.first = -dir.second;
      dir.second = temp_dir;
    }
    return neighbors;
  };

  auto EndCondition = [&](const std::vector<Coord>& path) -> bool {
    if (path.back() == end) {
      answer = path.size() - 1;  // Record number of steps, not nodes
      return true;
    }
    return false;
  };

  aoc::BFS<Coord>(start, GetNeighbors, [](const std::vector<Coord>& path) {}, EndCondition);

  return answer;
}

Coord PartTwo(Infoformat& info) {
  bool blocked = false;  // Track if path is blocked.
  std::set<Coord> corrupted_mem;
  std::set<Coord> path_nodes;  // Track nodes in current path
  Coord start = {0, 0};
  Coord end = {kMaxX, kMaxY};

  auto GetNeighbors = [&](const std::vector<Coord>& path) -> std::vector<Coord> {
    std::vector<Coord> neighbors;

    Coord curr_node = path.back();
    Coord dir = {0, 1};
    for (int i = 0; i < 4; ++i) {
      Coord poss_neighbor = {curr_node.first + dir.first, curr_node.second + dir.second};
      if (IsOnMap(poss_neighbor) && !corrupted_mem.count(poss_neighbor)) {
        neighbors.push_back(poss_neighbor);
      }
      // Rotate dir 90 deg
      int temp_dir = dir.first;
      dir.first = -dir.second;
      dir.second = temp_dir;
    }
    return neighbors;
  };

  auto EndCondition = [&](const std::vector<Coord>& path) -> bool {
    if (path.back() == end) {
      blocked = false;
      for (const Coord& coord : path) {
        path_nodes.insert(coord);
      }
      return true;
    }
    return false;
  };

  Coord recent_coord;
  for (int i = 0; i < info.size() && !blocked; ++i) {
    corrupted_mem.insert(info[i]);
    recent_coord = info[i];
    // Don't check new corrupted memory spots if a path is still intact
    if (!path_nodes.count(info[i]) && i != 0) {
      continue;
    }
    path_nodes.clear();
    blocked = true;
    aoc::BFS<Coord>(start, GetNeighbors, [](const std::vector<Coord>& path) {}, EndCondition);
  }

  return recent_coord;
}

int main() {
  auto start = std::chrono::steady_clock::now();
  Infoformat info = GetInfo();
  auto read_done = std::chrono::steady_clock::now();

  int answer_one = PartOne(info);
  auto part_one_done = std::chrono::steady_clock::now();

  Coord answer_two = PartTwo(info);
  auto part_two_done = std::chrono::steady_clock::now();

  std::cout << "Part One Answer: " << answer_one << std::endl;
  std::cout << "Part Two Answer: " << answer_two.first << "," << answer_two.second << std::endl;

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
