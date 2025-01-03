#include "utils.h"

namespace aoc {
  
std::vector<std::string> split(std::string line,
                    std::string delim,
                    bool include_empty_lines,
                    bool remove_leading_space) {
  std::vector<std::string> parsed_line;
  // Continue iterating over string as long as a delimeter exists
  while (line.find(delim) != std::string::npos) {
    std::string sub_line = line.substr(0, line.find(delim));
    if (remove_leading_space) {
      while (sub_line.length() > 0 && (sub_line[0] == ' ' || sub_line[0] == '\t')) {
        sub_line = sub_line.substr(1);
      }
    }
    // Only save empty lines if the include_empty_lines flag is set to true
    if (sub_line != "" || include_empty_lines) {
      parsed_line.push_back(sub_line);
    }
    line = line.substr(line.find(delim) + delim.length());
  }
  // Once we have gone through all delimeters, process the final string
  if (remove_leading_space) {
    while (line.length() > 0 && (line[0] == ' ' || line[0] == '\t')) {
      line = line.substr(1);
    }
  }
  if (line != "" || include_empty_lines) {
    parsed_line.push_back(line);
  }
  return parsed_line;
}

bool IsInt(std::string str) {
  for (int i = 0; i < str.length(); ++i) {
    if (!isdigit(str[i]) && (i != 0 || str[i] != '-')) {
      return false;
    }
  }
  return true;
}

std::vector<int> GetInts(const std::vector<std::string>& strings) {
  std::vector<int> ints;
  for (const std::string& s : strings) {
    if (IsInt(s)) {
      ints.push_back(stoi(s));
    }
  }
  return ints;
}

std::vector<long> GetLongs(const std::vector<std::string>& strings) {
  std::vector<long> longs;
  for (const std::string& s : strings) {
    if (IsInt(s)) {
      longs.push_back(stol(s));
    }
  }
  return longs;
}

std::vector<long long> GetLLongs(const std::vector<std::string>& strings) {
  std::vector<long long> llongs;
  for (const std::string& s : strings) {
    if (IsInt(s)) {
      llongs.push_back(stoll(s));
    }
  }
  return llongs;
}

std::vector<unsigned long> GetULongs(const std::vector<std::string>& strings) {
  std::vector<unsigned long> ulongs;
  for (const std::string& s : strings) {
    if (IsInt(s)) {
      ulongs.push_back(stoul(s));
    }
  }
  return ulongs;
}

std::vector<unsigned long long> GetULLongs(const std::vector<std::string>& strings) {
  std::vector<unsigned long long> ullongs;
  for (const std::string& s : strings) {
    if (IsInt(s)) {
      ullongs.push_back(stoull(s));
    }
  }
  return ullongs;
}

long gcd(long a, long b) {
  if (b == 0)
    return a;
  return gcd(b, a % b);
}
 
long lcm(std::vector<int> nums) {
  long lcm = nums[0];
  for (size_t i = 1; i < nums.size(); i++) {
    lcm = (((nums[i] * lcm)) / (gcd(nums[i], lcm)));
  }
  return lcm;
}

// returns a (mod m) always in the range from 0 to m - 1
int PosMod(int a, int b) {
  return (a % b + b) % b;
}

std::vector<std::string> FindAllRegexStrings(std::string input, std::string pattern) {
  std::regex regex_pattern(pattern);

  // Create a regex iterator
  std::sregex_iterator begin(input.begin(), input.end(), regex_pattern);
  std::sregex_iterator end;

  // Store matches in a vector
  std::vector<std::string> matches;
  for (auto it = begin; it != end; ++it) {
    matches.push_back(it->str());
  }
  return matches;
}

std::vector<std::smatch> FindAllRegexMatches(std::string input, std::string pattern) {
  std::regex regex_pattern(pattern);

  // Create a regex iterator
  std::sregex_iterator begin(input.begin(), input.end(), regex_pattern);
  std::sregex_iterator end;

  // Store matches in a vector
  std::vector<std::smatch> matches;
  for (auto it = begin; it != end; ++it) {
    matches.push_back(*it);
  }
  return matches;
}

bool ExactRegexMatch(std::string input, std::string pattern) {
  std::regex regex_pattern(pattern);

  // Check for an exact match
  return std::regex_match(input, regex_pattern);
}

// ------------------ BarrierMap definitions below ----------------------

bool BarrierMap::IsOutside(const Coord& coord) const {
  return coord.first < 0 || coord.first >= length ||
        coord.second < 0 || coord.second >= height;
}

BarrierMap::SpaceType BarrierMap::MovePlayer() {
  Coord new_loc = {pos.first + dir.first,
                  pos.second + dir.second};
  if (IsOpen(new_loc)) {
    pos = new_loc;
    return SpaceType::kOpen;
  }
  
  return IsBarrier(new_loc) ? SpaceType::kBarrier : SpaceType::kOffMap;
}

void BarrierMap::TurnRight() {
  int new_x_vel = -dir.second;
  int new_y_vel = dir.first;

  dir.first = new_x_vel;
  dir.second = new_y_vel;
}

void BarrierMap::TurnLeft() {
  int new_x_vel = dir.second;
  int new_y_vel = -dir.first;

  dir.first = new_x_vel;
  dir.second = new_y_vel;
}

}