#include "utils.h"

typedef std::vector<std::string> infoformat;

infoformat GetInfo() {
  infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  while (std::getline(input, line)) {
    info.push_back(line);
  }
  return info;
}

// Gets lines in all directions
infoformat GetAllLines(infoformat& board) {
  // Add horizontal lines
  infoformat all_lines = board;

  // Add backwards horizontal lines
  for (std::string line : board) {
    std::reverse(line.begin(), line.end());
    all_lines.push_back(line);
  }

  // Add vrtical and backwards vertical lines
  for (int i = 0; i < board[0].size(); ++i) {
    std::string col;
    std::string backwards_col;
    for (int j = 0; j < board.size(); ++j) {
      col += board[j][i];
      backwards_col = board[j][i] + backwards_col;
    }
    all_lines.push_back(col);
    all_lines.push_back(backwards_col);
  }

  // Add all diagonals
  for (int i = 0; i < board.size() + board[0].size() - 1; ++i) {
    // Track position of diagonals moving down and right (or up and left)
    // Position is going to start on the left side or on the top
    std::pair<int, int> pos_dr = {0, 0};
    if (i < board.size()) {
      pos_dr.first = board.size() - 1 - i;
    } else {
      pos_dr.second = i - board.size() + 1;
    }

    // Track position of diagonals down and left (or up and right)
    // Position is going to start on the top or on the right side
    std::pair<int, int> pos_dl = {0, 0};
    if (i < board[0].size()) {
      pos_dl.second = i;
    } else {
      pos_dl.first = i - board[0].size() + 1;
      pos_dl.second = board[0].size() - 1;
    }

    // Create down and right diagonals
    std::string diag_dr;
    std::string backwards_diag_dr;

    while (pos_dr.first < board.size() && pos_dr.second < board[0].size()) {
      diag_dr += board[pos_dr.first][pos_dr.second];
      backwards_diag_dr = board[pos_dr.first][pos_dr.second] + backwards_diag_dr;
      ++pos_dr.first;
      ++pos_dr.second;
    }

    // Create down and left diagonals
    std::string diag_dl;
    std::string backwards_diag_dl;

    while (pos_dl.first < board.size() && pos_dl.second >= 0) {
      diag_dl += board[pos_dl.first][pos_dl.second];
      backwards_diag_dl = board[pos_dl.first][pos_dl.second] + backwards_diag_dl;
      ++pos_dl.first;
      --pos_dl.second;
    }

    // Add diagonals if they are longer than 4 (the length of XMAS)
    if (diag_dr.size() >= 4) {
      all_lines.push_back(diag_dr);
      all_lines.push_back(backwards_diag_dr);
    }

    if (diag_dl.size() >= 4) {
      all_lines.push_back(diag_dl);
      all_lines.push_back(backwards_diag_dl);
    }
  }

  return all_lines;
}

// Expects a 9 character string representing a 3x3 grid.
// Will check for the X-MAS pattern
bool IsXMAS(std::string candidate) {
  std::string pattern1 = "M.M.A.S.S";
  std::string pattern2 = "S.M.A.S.M";
  std::string pattern3 = "M.S.A.M.S";
  std::string pattern4 = "S.S.A.M.M";

  return aoc::ExactRegexMatch(candidate, pattern1) ||
        aoc::ExactRegexMatch(candidate, pattern2) ||
        aoc::ExactRegexMatch(candidate, pattern3) ||
        aoc::ExactRegexMatch(candidate, pattern4);
}

// Returns 3x3 grid as a string of length 9 with top left starting at start
// or empty string if grid does not fit
std::string GetGrid(std::pair<int, int> start, infoformat& board) {
  std::string grid = "";

  if (start.first + 2 >= board.size() || start.second + 2 >= board[0].size()) {
    return grid;
  }

  for (int i = start.first; i < start.first + 3; ++i) {
    for (int j = start.second; j < start.second + 3; ++j) {
      grid += board[i][j];
    }
  }

  return grid;
}

int PartOne(infoformat& info) {
  int answer = 0;
  infoformat all_lines = GetAllLines(info);

  // Use regex to find all occurrences of XMAS
  std::string pattern = "XMAS";
  for (std::string& line : all_lines) {
   answer += aoc::FindAllRegex(line, pattern).size();
  }

  return answer;
}

int PartTwo(infoformat& info) {
  int answer = 0;
  for (int i = 0; i < info.size(); ++i) {
    for (int j = 0; j < info[0].size(); ++j) {
      std::string grid = GetGrid({i, j}, info);
      answer += (grid != "" && IsXMAS(grid));
    }
  }
  return answer;
}

int main() {
  infoformat info = GetInfo();
  int answer_one = PartOne(info);
  int answer_two = PartTwo(info);
  std::cout << "Part One: " << answer_one << std::endl;
  std::cout << "Part Two: " << answer_two << std::endl;
  return 0;
}
