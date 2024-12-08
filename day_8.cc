#include "utils.h"

using Coord = std::pair<int, int>;
using Infoformat = std::tuple<int, int, std::map<char, std::vector<Coord>>>;

Infoformat GetInfo() {
  Infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  int y = 0;
  int length = 0;
  int height = 0;
  while (std::getline(input, line)) {
    for (int x = 0; x < line.size(); ++x) {
      char c = line[x];
      if (c == '.') {
        continue;
      }
      std::get<2>(info)[c].emplace_back(x, y);
    }
    length = line.size();
    ++y;
  }
  height = y;
  std::get<0>(info) = length;
  std::get<1>(info) = height;
  return info;
}

// Takes all positions of one antenna type. Calculates antinodes.
// Then adds them to antinodes argument.
void AddAntinodes(int length,
                  int height,
                  const std::vector<Coord>& antenna_locs,
                  std::set<Coord>* antinodes) {
  for (int i = 0; i < antenna_locs.size() - 1; ++i) {
    for (int j = i + 1; j < antenna_locs.size(); ++j) {
      // Get two coords to compare
      Coord antenna1 = antenna_locs[i];
      Coord antenna2 = antenna_locs[j];

      // Get run and rise. These are used to place the antinodes
      int run = antenna2.first - antenna1.first;
      int rise = antenna2.second - antenna1.second;
      Coord antinode1 = {antenna1.first - run, antenna1.second - rise};
      Coord antinode2 = {antenna2.first + run, antenna2.second + rise};

      // Check if antinode1 is on the map
      if (antinode1.first >= 0 && antinode1.first < length &&
          antinode1.second >= 0 && antinode1.second < height) {
        antinodes->insert(antinode1);
      }
      // Check if antinode2 is on the map
      if (antinode2.first >= 0 && antinode2.first < length &&
          antinode2.second >= 0 && antinode2.second < height) {
        antinodes->insert(antinode2);
      }
    }
  }
}

// Get all points on the line formed by two antennas. Takes one of the antennas
// and the slope to generate all points formed by the line.
void AddAllPointsOnLine(int length,
                        int height,
                        Coord antenna,
                        int run,
                        int rise,
                        std::set<Coord>* antinodes) {
  // Calculate run and rise to generate all points on the line
  bool is_on_map = true;
  Coord antinode = antenna;
  // Add all points in positive direction
  while (is_on_map) {
    antinodes->insert(antinode);
    antinode = {antinode.first + run, antinode.second + rise};
    is_on_map = antinode.first >= 0 && antinode.first < length &&
                antinode.second >= 0 && antinode.second < height;
  }

  // Reset antinode tracker and is_on_map
  antinode = antenna;
  is_on_map = true;
  // Add all points in negative direction
  while (is_on_map) {
    antinodes->insert(antinode);
    antinode = {antinode.first - run, antinode.second - rise};
    is_on_map = antinode.first >= 0 && antinode.first < length &&
                antinode.second >= 0 && antinode.second < height;
  }
}

// Takes all positions of one antenna type. Calculates antinodes.
// Then adds them to antinodes argument.
void AddAllAntinodes(int length,
                    int height,
                    const std::vector<Coord>& antenna_locs,
                    std::set<Coord>* antinodes) {
  for (int i = 0; i < antenna_locs.size() - 1; ++i) {
    for (int j = i + 1; j < antenna_locs.size(); ++j) {
      // Get two coords to compare
      Coord antenna1 = antenna_locs[i];
      Coord antenna2 = antenna_locs[j];

      int run = antenna2.first - antenna1.first;
      int rise = antenna2.second - antenna1.second;
      
      AddAllPointsOnLine(length, height, antenna1, run, rise, antinodes);
    }
  }
}

int PartOne(int length, int height, const std::map<char, std::vector<Coord>>& antennas) {
  std::set<Coord> antinodes;
  // Iterate through all antenna groups and calculate all antinodes formed
  // by each pair
  for (const auto& antennas : antennas) {
     std::vector<Coord> antenna_locs = antennas.second;
     AddAntinodes(length, height, antenna_locs, &antinodes);
  }
  return antinodes.size();
}

int PartTwo(int length, int height, const std::map<char, std::vector<Coord>>& antennas) {
  std::set<Coord> antinodes;
  // Iterate through all antenna groups and calculate all antinodes formed
  // by each pair
  for (const auto& antennas : antennas) {
     std::vector<Coord> antenna_locs = antennas.second;
     AddAllAntinodes(length, height, antenna_locs, &antinodes);
  }
  return antinodes.size();
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
    std::chrono::duration<double, std::milli>>(read_done - start).count();
  double part_one_time = std::chrono::duration_cast<
    std::chrono::duration<double, std::milli>>(part_one_done - read_done).count();
  double part_two_time = std::chrono::duration_cast<
    std::chrono::duration<double, std::milli>>(part_two_done - part_one_done).count();
  std::cout << "\n------------Time analysis------------\n"
            << "READ TIME: " << read_time << " ms\n"
            << "PART ONE TIME: " << part_one_time << " ms\n"
            << "PART TWO TIME: " << part_two_time << " ms" << std::endl;

  return 0;
}
