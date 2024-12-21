#include "utils.h"

using Maze = std::vector<std::string>;
using Coord = std::pair<int, int>;
using Infoformat = std::tuple<Maze, Coord, Coord>;

Infoformat GetInfo() {
  Maze maze;
  Coord start;
  Coord end;
  std::ifstream input("input.txt");
  std::string line;
  int row = 0;
  while (std::getline(input, line)) {
    std::string maze_line;
    for (int col = 0; col < line.size(); ++col) {
      if (line[col] == 'S') {
        start = {row, col};
        maze_line += '.';
      } else if (line[col] == 'E') {
        end = {row, col};
        maze_line += '.';
      } else {
        maze_line += line[col];
      }
    }
    ++row;
    maze.push_back(maze_line);
  }
  return {maze, start, end};
}

bool IsOnMap(int height, int width, const Coord& pos) {
  return (pos.first >= 0 &&
          pos.first < height &&
          pos.second >= 0 &&
          pos.second < width);
}

// Recursively get all skips in a certain direction
void GetSkipsInDir(const Maze& maze,
                  const Coord& pos,
                  Coord dir,
                  int cheat_length,
                  bool has_turned,
                  std::vector<Coord>* skips) {
  int height = maze.size();
  int width = maze[0].size();

  // If all cheat time has been used up, stop
  if (cheat_length == 0) {
    return;
  }

  for (int j = 0; j < 2; ++j) {
    if (j == 1 && has_turned) {
      return;
    } else if (j == 1) {
      // Rotate dir by 90 deg
      int temp_dir = dir.first;
      dir.first = -dir.second;
      dir.second = temp_dir;
      has_turned = true;
    }
    Coord new_pos = {pos.first + dir.first, pos.second + dir.second};

    if (!IsOnMap(height, width, new_pos)) {
      continue;
    }
    if (maze[new_pos.first][new_pos.second] == '.') {
      skips->push_back(new_pos);
    }
    // Perform recursive calls to generate all reachable positions
    GetSkipsInDir(maze, new_pos, dir, cheat_length - 1, has_turned, skips);
  }
}

// Looks at all possible coords to get to using a cheat
std::vector<Coord> GetSkips(const Maze& maze, const Coord& pos, int cheat_length) {
  std::vector<Coord> skips;
  int height = maze.size();
  int width = maze[0].size();

  Coord dir = {0, 1};
  for (int i = 0; i < 4; ++i) {
    Coord new_pos = {pos.first + dir.first, pos.second + dir.second};
    if (IsOnMap(height, width, new_pos)) {
      // No need to add a one step "skip" since its equivalent to a normal step
      GetSkipsInDir(maze, new_pos, dir, cheat_length - 1, false, &skips);
    }
    // Rotate dir by 90 deg
    int temp_dir = dir.first;
    dir.first = -dir.second;
    dir.second = temp_dir;
  }
  return skips;
}

void GetDistances(const Maze& maze,
                  const Coord& start,
                  const Coord& end,
                  std::map<Coord, int>* dist_from_start) {
  int height = maze.size();
  int width = maze[0].size();

  Coord pos = start;
  int depth = 0;

  (*dist_from_start)[pos] = depth;
  Coord dir = {0, 1};

  while (pos != end) {
    ++depth;
    // Flip the direction we just moved in and start rotating from there so that
    // we will not re-check the position we moved from
    dir.first = -dir.first;
    dir.second = -dir.second;

    for (int i = 0; i < 3; ++i) {
      // Rotate dir by 90 deg
      int temp_dir = dir.first;
      dir.first = -dir.second;
      dir.second = temp_dir;
      // Check position in direction of dir
      Coord new_pos = {pos.first + dir.first, pos.second + dir.second};
      if (IsOnMap(height, width, pos) && maze[new_pos.first][new_pos.second] == '.') {
        (*dist_from_start)[new_pos] = depth;
        pos = new_pos;  // Update pos
        break;  // After finding next step, break out of for loop
      }
    }
  }
}

int PartOne(const Maze& maze, const Coord& start, const Coord& end) {
  int answer = 0;
  std::map<Coord, int> dist_from_start;

  GetDistances(maze, start, end, &dist_from_start);

  for (auto [pos, dist] : dist_from_start) {
    for (const Coord& skip : GetSkips(maze, pos, 2)) {
      // Subtract 2 since a skip still takes 2 picoseconds
      if (dist_from_start[skip] - dist - 2 >= 100) {
        ++answer;
      }
    }
  }
  return answer;
}

// 1061108 is too high
int PartTwo(const Maze& maze, const Coord& start, const Coord& end) {
  int answer = 0;
  std::map<Coord, int> dist_from_start;

  GetDistances(maze, start, end, &dist_from_start);

  for (auto [pos, dist] : dist_from_start) {
    for (const Coord& skip : GetSkips(maze, pos, 20)) {
      // Subtract distance travelled during the skip to account for skip time
      int skip_dist = std::abs(pos.first - skip.first) + std::abs(pos.second - skip.second);
      if (dist_from_start[skip] - dist - skip_dist >= 100) {
        ++answer;
      }
    }
  }
  return answer;
}

int main() {
  auto start = std::chrono::steady_clock::now();
  Infoformat info = GetInfo();
  auto read_done = std::chrono::steady_clock::now();

  int answer_one = PartOne(std::get<0>(info), std::get<1>(info), std::get<2>(info));
  auto part_one_done = std::chrono::steady_clock::now();

  int answer_two = PartTwo(std::get<0>(info), std::get<1>(info), std::get<2>(info));
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
