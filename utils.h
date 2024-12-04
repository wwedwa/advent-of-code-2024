#ifndef UTILS_H
#define UTILS_H

// All of this is included just so I don't have to include it separately in every day
#include <iostream>
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
std::vector<int> GetInts(std::vector<std::string> strings);

// Gets all longs from a vector of strings
std::vector<long> GetLongs(std::vector<std::string> strings);

// Gets all long longs from a vector of strings
std::vector<long long> GetLLongs(std::vector<std::string> strings);

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
}

#endif // UTILS_H