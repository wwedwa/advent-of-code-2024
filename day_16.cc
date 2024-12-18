#include "utils.h"

using Coord = std::pair<int, int>;
// Store direction and location of deer
using DeerState = std::pair<Coord, Coord>;
using Maze = std::vector<std::string>;
// Store start state, maze, and end goal
using Infoformat = std::tuple<DeerState, Maze, Coord>;

Infoformat GetInfo() {
  DeerState start;
  Maze maze;
  Coord end;

  std::ifstream input("input.txt");
  std::string line;
  int row = 0;
  while (std::getline(input, line)) {
    std::string maze_line;
    for (int col = 0; col < line.size(); ++col) {
      if (line[col] == 'E') {
        end = {row, col};
        maze_line += '.';
      } else if (line[col] == 'S') {
        start = {{row, col}, {0, 1}};
        maze_line += '.';
      } else {
        maze_line += line[col];
      }
    }
    maze.push_back(maze_line);
    ++row;
  }
  return {start, maze, end};
}

std::map<DeerState, int> GetNeighbors(const Maze& maze, const DeerState& curr_node) {
  std::map<DeerState, int> neighbors;
  // Get the state of the deer
  Coord loc = curr_node.first;
  Coord dir = curr_node.second;

  for (int i = 0; i < 4; ++i) {
    Coord new_loc = {loc.first + dir.first, loc.second + dir.second};
    if (maze[new_loc.first][new_loc.second] == '.') {
      int turn_penalty = 0;
      if (i == 1 || i == 3) {
        turn_penalty = 1000;
      } else if (i == 2) {
        turn_penalty = 2000;
      }
      neighbors[{new_loc, dir}] = 1 + turn_penalty;
    }
    // Rotate 90 deg
    int temp_dir = dir.first;
    dir.first = -dir.second;
    dir.second = temp_dir;
  }
  return neighbors;
}

// Use BFS to count all nodes which occur in the paths from start to finish
std::set<Coord> GetWalkedNodes(const aoc::ExDijkstraState<DeerState>& state, const Coord& end, int min_dist) {
  std::set<Coord> all_nodes;
  const auto& prev_nodes = state.prev_nodes;

  auto GetNeighbors = [&](const std::vector<DeerState>& path) -> std::vector<DeerState> {
    all_nodes.insert(path.back().first);
    std::vector<DeerState> neighbors;
    
    if (!prev_nodes.count(path.back())) {
      return neighbors;
    }
    for (const DeerState& neighbor : prev_nodes.at(path.back())) {
      neighbors.push_back(neighbor);
    }
    return neighbors;
  };

  // For each possible end configuration, perform BFS to find all travelled
  // nodes if its distance is the min dist
  Coord dir = {0, 1};
  for (int i = 0; i < 4; ++i) {
    if (state.dists.count({end, dir}) && state.dists.at({end, dir}) == min_dist) {
      aoc::BFS<DeerState>({end, dir}, GetNeighbors);
    }
    int temp_dir = dir.first;
    dir.first = -dir.second;
    dir.second = temp_dir;
  }

  return all_nodes;
}

int PartOne(Infoformat& info) {
  int answer = 0;

  DeerState start = std::get<0>(info);
  Maze maze = std::get<1>(info);
  Coord end = std::get<2>(info);

  auto GetNeihgborsFunc = [&maze](const aoc::DijkstraState<DeerState>& state) {
    return GetNeighbors(maze, state.curr_node);
  };

  auto EndCondition = [&](const aoc::DijkstraState<DeerState>& state) -> bool {
    if (state.curr_node.first == end) {
      answer = state.dists.at(state.curr_node);
      return true;
    }
    return false;
  };

  aoc::Dijkstra<DeerState>(start,
                          GetNeihgborsFunc,
                          [](const aoc::DijkstraState<DeerState>&) {},
                          EndCondition);
  return answer;
}

int PartTwo(Infoformat& info, int min_dist) {
  DeerState start = std::get<0>(info);
  Maze maze = std::get<1>(info);
  Coord end = std::get<2>(info);

  auto GetNeihgborsFunc = [&maze](const aoc::ExDijkstraState<DeerState>& state) {
    return GetNeighbors(maze, state.curr_node);
  };

  // State will be subgraph of all shortest paths from start to all other nodes
  aoc::ExDijkstraState<DeerState> state = aoc::ExtendedDijkstra<DeerState>(start, GetNeihgborsFunc);

  std::set<Coord> walked = GetWalkedNodes(state, end, min_dist);
  return walked.size();
}

int main() {
  auto start = std::chrono::steady_clock::now();
  Infoformat info = GetInfo();
  auto read_done = std::chrono::steady_clock::now();

  int answer_one = PartOne(info);
  auto part_one_done = std::chrono::steady_clock::now();

  int answer_two = PartTwo(info, answer_one);
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
