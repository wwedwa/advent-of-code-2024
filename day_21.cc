#include "utils.h"

using Coord = std::pair<int, int>;
using Infoformat = std::vector<std::string>;

std::unordered_map<char, Coord> numeric_keypad;
std::unordered_map<char, Coord> arrow_keypad;

void InitiateKeypads() {
  numeric_keypad['7'] = {0, 0};
  numeric_keypad['8'] = {1, 0};
  numeric_keypad['9'] = {2, 0};
  numeric_keypad['4'] = {0, 1};
  numeric_keypad['5'] = {1, 1};
  numeric_keypad['6'] = {2, 1};
  numeric_keypad['1'] = {0, 2};
  numeric_keypad['2'] = {1, 2};
  numeric_keypad['3'] = {2, 2};
  numeric_keypad['.'] = {0, 3};
  numeric_keypad['0'] = {1, 3};
  numeric_keypad['A'] = {2, 3};

  arrow_keypad['.'] = {0, 0};
  arrow_keypad['^'] = {1, 0};
  arrow_keypad['A'] = {2, 0};
  arrow_keypad['<'] = {0, 1};
  arrow_keypad['v'] = {1, 1};
  arrow_keypad['>'] = {2, 1};
}

Infoformat GetInfo() {
  Infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  while (std::getline(input, line)) {
    info.push_back(line);
  }
  return info;
}

// Gets the direct path(s) from the start to the end button. For example, if two
// buttons are (x,y) distance apart, we will first step x steps in the x direction,
// and then y steps in the y direction. If the blank button ('.') is on one of
// these paths, we will only return the other path
std::vector<std::string> GetDirectPaths(const Coord& start, const Coord& end, const std::unordered_map<char, Coord>& keypad) {
  std::vector<std::string> paths;

  int min_x = std::min(start.first, end.first);
  int min_y = std::min(start.second, end.second);

  int max_x = std::max(start.first, end.first);
  int max_y = std::max(start.second, end.second);

  int delta_x = end.first - start.first;
  int delta_y = end.second - start.second;

  char x_dir = (delta_x < 0) ? '<' : '>';
  char y_dir = (delta_y < 0) ? '^' : 'v';

  std::string x_move(std::abs(delta_x), x_dir);
  std::string y_move(std::abs(delta_y), y_dir);

  std::string xy_path = x_move + y_move + "A";
  std::string yx_path = y_move + x_move + "A";

  Coord blank_loc = keypad.at('.');
  bool add_xy_path = start.second != blank_loc.second || blank_loc.first < min_x || blank_loc.first > max_x;
  bool add_yx_path = start.first != blank_loc.first || blank_loc.second < min_y || blank_loc.second > max_y;

  // We add A at the end since for any button to be pressed, A must be pressed on the robots control pad
  if (add_xy_path) {
    paths.push_back(xy_path);
  }

  if (add_yx_path && xy_path != yx_path) {
    paths.push_back(yx_path);
  }

  return paths;
}

// Return all shortest combos of button presses to get the desired input
std::vector<std::string> GetShortestPaths(const std::string& input, const std::unordered_map<char, Coord>& keypad) {
  std::vector<std::string> paths;
  char curr_button = 'A';  // Every robot starts on the A button
  for (int i = 0; i < input.size(); ++i) {
    std::vector<std::string> direct_paths = GetDirectPaths(keypad.at(curr_button), keypad.at(input[i]), keypad);
    std::vector<std::string> new_paths;
    for (const std::string& path : direct_paths) {
      if (i == 0) {
        new_paths.push_back(path);
      } else {
        for (const std::string& old_path : paths) {
          new_paths.push_back(old_path + path);
        }
      }
    }
    paths = new_paths;
    curr_button = input[i];
  }
  return paths;
}

void GetShortestPathLength(std::string input, int depth, std::unordered_map<char, Coord> keypad, int* length) {
  if (depth == 0 && (input.size() < *length || *length == -1)) {
    *length = input.size();
    return;
  } else if (depth == 0) {
    return;
  }

  for (std::string path : GetShortestPaths(input, keypad)) {
    GetShortestPathLength(path, depth - 1, arrow_keypad, length);
  }
}

int PartOne(const Infoformat& info) {
  int answer = 0;
  for (std::string input : info) {
    int length = -1;
    GetShortestPathLength(input, 26, numeric_keypad, &length);
    answer += std::stoi(input.substr(0, input.size() - 1)) * length;
  }
  return answer;
}

int PartTwo(const Infoformat& info) {
  int answer = 0;
  return answer;
}

int main() {
  InitiateKeypads();
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
