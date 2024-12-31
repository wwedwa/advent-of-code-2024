#include "utils.h"

using Coord = std::pair<int, int>;
using Infoformat = std::vector<std::string>;
using ullong = unsigned long long;

std::unordered_map<char, Coord> numeric_keypad;
std::unordered_map<char, Coord> arrow_keypad;
// Key is a possible step (i.e. "<v", "A^", or "14") and value is the arrow pad
// key presses required to get from the first character to the second character
std::unordered_map<std::string, std::vector<std::string>>one_step_cache;

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

// Takes an input string and returns all steps needed to complete the input
// e.g. "029A" becomes {"02", "29", "9A"}.
std::vector<std::string> GetSteps(std::string input) {
  std::vector<std::string> steps;

  for (int i = 0; i < input.size() - 1; ++i) {
    steps.push_back(input.substr(i, 2));
  }

  return steps;
}

// Gets the direct (quickest) path from start to end on one of the keypads.
// Note there are multiple of these paths, but this function is optimized enough
// to only return one.
std::vector<std::string> GetDirectSteps(const Coord& start, const Coord& end, const std::unordered_map<char, Coord>& keypad) {
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

  // Add "A" at beginning and end because each robot starts on A for each
  // move (since we must've just pressed a button before) and it ends on A
  // to press the next button
  std::string xy_path = "A" + x_move + y_move + "A";
  std::string yx_path = "A" + y_move + x_move + "A";

  Coord blank_loc = keypad.at('.');
  bool add_xy_path = start.second != blank_loc.second || blank_loc.first < min_x || blank_loc.first > max_x;
  bool add_yx_path = start.first != blank_loc.first || blank_loc.second < min_y || blank_loc.second > max_y;

  // The last condition in the AND statement assumes no optimal path will visit
  // the < arrow last since this is the furthest from the A button. Thus it will
  // be optimal to visit it first, and then visit the other buttons on the way
  // back to A.
  if ((add_yx_path && xy_path != yx_path && x_dir != '<') || !add_xy_path) {
    return GetSteps(yx_path);
  } else {
    return GetSteps(xy_path);
  }
}

void InitiateOneStepCache() {
  for (auto [key1, val1] : numeric_keypad) {
    if (key1 == '.') continue;
    for (auto [key2, val2] : numeric_keypad) {
      if (key2 == '.') continue;
      std::string move{key1, key2};
      one_step_cache[move] = GetDirectSteps(val1, val2, numeric_keypad);
    }
  }
  for (auto [key1, val1] : arrow_keypad) {
    if (key1 == '.') continue;
    for (auto [key2, val2] : arrow_keypad) {
      if (key2 == '.') continue;
      std::string move{key1, key2};
      one_step_cache[move] = GetDirectSteps(val1, val2, arrow_keypad);
    }
  }
}

// Idea: count the number of step types in each path. Then when expanding a 
// path for the next robot, expand each step and count the resulting steps
// (A step is two buttons, a starting button and ending button like 02, vv, etc)
ullong GetShortestPathLength(int depth,
                            std::unordered_map<std::string, ullong> step_count,
                            const std::unordered_map<char, Coord>& keypad) {
  if (depth == 0) {
    ullong count = 0;
    for (auto [key, val] : step_count) {
      count += val;
    }
    return count;
  }

  std::unordered_map<std::string, ullong> next_step_count;
  for (auto [key, val] : step_count) {
    Coord start = keypad.at(key[0]);
    Coord end = keypad.at(key[1]);
    for (std::string step : GetDirectSteps(start, end, keypad)) {
      next_step_count[step] += val;
    }
  }

  return GetShortestPathLength(depth - 1, next_step_count, arrow_keypad);
}

ullong PartOne(const Infoformat& info) {
  ullong answer = 0;
  for (std::string input : info) {
    input = "A" + input;
    std::unordered_map<std::string, ullong> step_count;
    for (const std::string& steps : GetSteps(input)) {
      step_count[steps] += 1;
    }
    answer += std::stoi(input.substr(1, input.size() - 1)) * GetShortestPathLength(3, step_count, numeric_keypad);
  }
  return answer;
}

ullong PartTwo(const Infoformat& info) {
  ullong answer = 0;
  for (std::string input : info) {
    input = "A" + input;
    std::unordered_map<std::string, ullong> step_count;
    for (const std::string& steps : GetSteps(input)) {
      step_count[steps] += 1;
    }
    answer += std::stoi(input.substr(1, input.size() - 1)) * GetShortestPathLength(26, step_count, numeric_keypad);
  }
  return answer;
}

int main() {
  std::string str;
  InitiateKeypads();
  InitiateOneStepCache();
  auto start = std::chrono::steady_clock::now();
  Infoformat info = GetInfo();
  auto read_done = std::chrono::steady_clock::now();

  ullong answer_one = PartOne(info);
  auto part_one_done = std::chrono::steady_clock::now();

  ullong answer_two = PartTwo(info);
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
