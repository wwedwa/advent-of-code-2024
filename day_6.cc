#include "utils.h"

class GuardMap : public aoc::BarrierMap {
 public:
  Coord GetSpotInfront() {
    return Coord(pos.first + dir.first,
                pos.second + dir.second);
  }

  SpaceType MovePlayer() override {
    SpaceType space_type = BarrierMap::MovePlayer();
    if (space_type == SpaceType::kBarrier) {
      TurnRight();
    }
    return space_type;
  }
};

GuardMap GetInfo() {
  GuardMap map;
  std::ifstream input("input.txt");
  std::string line;
  int y = 0;
  while (std::getline(input, line)) {
    int x = 0;
    for (char c : line) {
      // Either set guard position or add spaces to map if they are empty
      if (c == '^') {
        map.SetPos({x, y});
        map.AddOpenSpace({x, y});
      } else if (c == '.') {
        map.AddOpenSpace({x, y});
      } else {
        map.AddBarrier({x, y});
      }
      ++x;  // Incrememnt x after finishing a character
    }
    map.length = x;
    ++y;  // Incrememnt x after finishing a line
  }
  map.height = y;
  map.SetDir({0, -1});

  return map;
}

// Don't take reference. This will change the map
std::set<std::pair<int, int>> GetPath(GuardMap map) {
  std::set<std::pair<int, int>> visited_spots;
  visited_spots.insert(map.GetPos());

  while (map.MovePlayer() != aoc::BarrierMap::SpaceType::kOffMap) {
    visited_spots.insert(map.GetPos());
  }
  return visited_spots;
}

bool TestCycle(GuardMap& map) {
  std::set<std::pair<std::pair<int, int>, std::pair<int, int>>> visited_spots;
  visited_spots.insert({map.GetPos(), map.GetDir()});

  while (map.MovePlayer() != aoc::BarrierMap::SpaceType::kOffMap) {
    if (visited_spots.count({map.GetPos(), map.GetDir()})) {
      return true;
    }
    visited_spots.insert({map.GetPos(), map.GetDir()});
  }
  return false;
}

int PartOne(GuardMap& map) {
  return GetPath(map).size();
}

int PartTwo(GuardMap& map) {
  int answer = 0;
  // We only need to test barriers on the original path since these are the 
  // only spaces that the guard travels to.
  std::set<std::pair<int, int>> visited_spots = GetPath(map);

  std::pair<int, int> orig_pos = map.GetPos();
  std::pair<int, int> orig_dir = map.GetDir();

  // Can't place new barrier at starting position
  visited_spots.erase(orig_pos);

  for (const std::pair<int, int>& coord : visited_spots) {
    // Add spot as barrier
    map.AddBarrier(coord);
    map.RemoveOpenSpace(coord);

    answer += TestCycle(map);
    // Re-correct map to original state
    map.RemoveBarrier(coord);
    map.AddOpenSpace(coord);
    map.SetPos(orig_pos);
    map.SetDir(orig_dir);
  }
  return answer;
}

int main() {
  GuardMap map = GetInfo();
  int answer_one = PartOne(map);
  int answer_two = PartTwo(map);
  std::cout << "Part One: " << answer_one << std::endl;
  std::cout << "Part Two: " << answer_two << std::endl;
  return 0;
}
