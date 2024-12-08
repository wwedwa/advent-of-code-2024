#ifndef UTILS_H
#define UTILS_H

// All of this is included just so I don't have to include it separately in every day
#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <deque>
#include <list>
#include <utility>
#include <tuple>
#include <algorithm>
#include <climits>
#include <numeric>
#include <regex>

namespace aoc {

// Split line into vector of string based on given delimiter
std::vector<std::string> split(std::string line,
                  std::string delim=" ",
                  bool include_empty_lines=false,
                  bool remove_leading_space=false);

// Checks if a given string is a number (specifically int)
bool IsInt(std::string num);

// Gets all ints from a vector of strings
std::vector<int> GetInts(const std::vector<std::string>& strings);

// Gets all longs from a vector of strings
std::vector<long> GetLongs(const std::vector<std::string>& strings);

// Gets all long longs from a vector of strings
std::vector<long long> GetLLongs(const std::vector<std::string>& strings);

// Gets all unsigned longs from a vector of strings
std::vector<unsigned long> GetULongs(const std::vector<std::string>& strings);

// Calculate gcd with Euclid's Algorithm
long gcd(long a, long b);

// Calculate lcm for a vector of ints
long lcm(std::vector<int> nums);

// Calculate a special mod
int mod(int m, int n);

// Find all instances of a regex pattern in the given string
std::vector<std::string> FindAllRegex(std::string input, std::string pattern);

// Returns true if the input and pattern are exact matches. Otherwise, false
bool ExactRegexMatch(std::string input, std::string pattern);



// Structure to represent a simple grid with barriers and a moveable player 
// since that is a common data structure in AoC. Assumes the top left coord 
// is 0,0. Right is positive x direction, down is positive y direction.
class BarrierMap {
 public:

  enum class SpaceType {
    kOpen,
    kBarrier,
    kOffMap
  };

  using Coord = std::pair<int, int>;
  using CoordSet = std::set<Coord>;

  Coord GetPos() { return pos; }

  Coord GetDir() { return dir; }

  void SetLength(int l) { length = l; }

  void SetHeight(int h) { height = h; }

  void SetPos(Coord player_pos) { pos = player_pos; }

  void SetDir(Coord player_dir) { dir = player_dir; }

  void AddOpenSpace(Coord open_space) { open_spaces.insert(open_space); }
  void RemoveOpenSpace(Coord open_space) { open_spaces.erase(open_space); }

  void AddBarrier(Coord barrier) { barriers.insert(barrier); }
  void RemoveBarrier(Coord barrier) { barriers.erase(barrier); }
 
  bool IsOutside(const Coord& coord) const;

  bool IsBarrier(const Coord& coord) const { return barriers.count(coord); }

  bool IsOpen(const Coord& coord) const { return open_spaces.count(coord); }
  
  // Attempts to move player. Returns the type of spot the player tried to move to
  virtual SpaceType MovePlayer();

  void TurnRight();

  void TurnLeft();

  int length;
  int height;

 protected:
  CoordSet open_spaces;
  CoordSet barriers;
  Coord pos;
  Coord dir;
};

}

#endif // UTILS_H