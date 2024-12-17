#include "utils.h"

using Coord = std::pair<int, int>;

// The border of the map is all barriers, so no need to check length or width
struct WarehouseMap {

  void UpdateMap() {
    for (const Coord& coord : coords_to_clear) {
      map[coord.first][coord.second] = '.';
    }
    for (auto [key, val] : coords_to_change) {
      map[key.first][key.second] = val;
    }
  }
  // For part 2
  bool HandleBigBoxes(Coord loc, Coord dir) {
    // offset decides which way we need to look for the other side of the box
    int offset = (map[loc.first][loc.second] == ']') ? -1 : 1;
    Coord new_loc1 = {loc.first + dir.first, loc.second + dir.second};
    Coord new_loc2 = {loc.first + dir.first, loc.second + dir.second + offset};
    if (MoveBoxes(new_loc1, dir) && MoveBoxes(new_loc2, dir)) {
      coords_to_change[new_loc1] = map[loc.first][loc.second];
      coords_to_change[new_loc2] = map[loc.first][loc.second + offset];
      coords_to_clear.insert(loc);
      coords_to_clear.insert({loc.first, loc.second + offset});
      return true;
    }
    return false;
  }

  bool MoveBoxes(Coord loc, Coord dir) {
    if (map[loc.first][loc.second] == '.') {
      return true;
    } else if (map[loc.first][loc.second] == '#') {
      return false;
    } else if (map[loc.first][loc.second] == 'O' || dir.second != 0) {
      // This case handles small boxes (those in part 1) or moving big boxes
      // horizontally since that is the same action
      Coord new_loc = {loc.first + dir.first, loc.second + dir.second};
      if (MoveBoxes(new_loc, dir)) {
        map[new_loc.first][new_loc.second] = map[loc.first][loc.second];
        map[loc.first][loc.second] = '.';
        return true;
      }
      return false;  // If box cannot move
    } else {
      return HandleBigBoxes(loc, dir);  // Handle big boxes for part 2
    }
  }

  void MoveRobot(char dir) {
    Coord coord_dir = {0, 0};
    if (dir == '>') {
      coord_dir = {0, 1};
    } else if (dir == '<') {
      coord_dir = {0, -1};
    } else if (dir == '^') {
      coord_dir = {-1, 0};
    } else {
      coord_dir = {1, 0};
    }
    Coord new_pos = {pos.first + coord_dir.first, pos.second + coord_dir.second};
    if (MoveBoxes(new_pos, coord_dir)) {
      UpdateMap();  // For part 2
      pos = new_pos;
    }
    coords_to_clear.clear();
    coords_to_change.clear();
  }

  std::vector<std::string> map;
  // for part 2. store what needs to change if all box movements succeed
  // We first clear all old boxes and then draw in the new boxes
  std::set<Coord> coords_to_clear;
  std::map<Coord, char> coords_to_change;
  Coord pos;
};

using Infoformat = std::pair<WarehouseMap, std::string>;

Infoformat GetInfo() {
  Infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  int row = 0;
  // Read map
  while (std::getline(input, line) && !line.empty()) {
    std::string map_line;
    for (int col = 0; col < line.size(); ++col) {
      if (line[col] == '@') {
        info.first.pos = {row, col};
        map_line.push_back('.');  // Robot starts on empty spot
      } else {
        map_line.push_back(line[col]);
      }
    }
    ++row;
    info.first.map.push_back(map_line);
  }

  // Read directions
  while (std::getline(input, line)) {
    info.second += line;
  }
  return info;
}

void PrintMap(const WarehouseMap& map) {
  for (int i = 0; i < map.map.size(); ++i) {
    for (int j = 0; j < map.map[0].size(); ++j) {
      if (map.pos.first == i && map.pos.second == j) {
        std::cout << '@';
      } else {
        std::cout << map.map[i][j];
      }
    }
    std::cout << std::endl;
  }
}

WarehouseMap ConvertToBigWarehouse(const WarehouseMap& map) {
  WarehouseMap new_map;
  for (int i = 0; i < map.map.size(); ++i) {
    std::string new_line;
    for (int j = 0; j < map.map[0].size(); ++j) {
      if (map.map[i][j] == '.') {
        new_line += "..";
      } else if (map.map[i][j] == 'O') {
        new_line += "[]";
      } else {
        new_line += "##";
      }
    }
    new_map.map.push_back(new_line);
  }
  new_map.pos = {map.pos.first, map.pos.second * 2};

  return new_map;
}

int PartOne(WarehouseMap map, const std::string& dirs) {
  int answer = 0;
  for (char dir : dirs) {
    map.MoveRobot(dir);
  }

  for (int i = 0; i < map.map.size(); ++i) {
    for (int j = 0; j < map.map[0].size(); ++j) {
      if (map.map[i][j] == 'O') {
        answer += i * 100 + j;
      }
    }
  }
  return answer;
}

long PartTwo(WarehouseMap map, const std::string& dirs) {
  long answer = 0;
  map = ConvertToBigWarehouse(map);
  for (char dir : dirs) {
    map.MoveRobot(dir);
  }

  for (int i = 0; i < map.map.size(); ++i) {
    for (int j = 0; j < map.map[0].size(); ++j) {
      if (map.map[i][j] == '[') {
        answer += i * 100 + j;
      }
    }
  }
  return answer;
}

int main() {
  auto start = std::chrono::steady_clock::now();
  Infoformat info = GetInfo();
  auto read_done = std::chrono::steady_clock::now();

  int answer_one = PartOne(info.first, info.second);
  auto part_one_done = std::chrono::steady_clock::now();

  long answer_two = PartTwo(info.first, info.second);
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
