#include "utils.h"

struct DiskObject {
  int size;
  // id is -1 for empty space, >=0 for files
  int id;
};

using Infoformat = std::pair<int, std::vector<DiskObject>>;

Infoformat GetInfo() {
  Infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  bool is_file = true;
  int curr_id = 0;
  while (std::getline(input, line)) {
    for (char c : line) {
      DiskObject disk_object;
      disk_object.size = c - '0';
      info.first += is_file * disk_object.size;  // Only add if on a file
      disk_object.id = is_file ? curr_id : -1;
      curr_id += is_file;  // Increment id only if we are on a file
      info.second.push_back(disk_object);
      is_file = !is_file;
    }
  }
  return info;
}

long CalculateProduct(int mem_index, int size, int id) {
  // Application of Gauss's sum formula
  return (long)id * (mem_index + mem_index + size - 1) * (size) / 2;
}

// Unweildy to use the DiskObject datatype for part 1. But I chose to go with it
// and it paid off for part 2 (if part 2 didn't exist, the best way is to break
// up the files into individual slices and move them that way like the question
// implies to do)
long PartOne(int size, std::vector<DiskObject> objs) {
  long answer = 0;
  // Remove last element if it is empty space
  if (objs.back().id == -1) {
    objs.pop_back();
  }
  // Two trackers. One starting at the front moving forward and vice versa
  int front_idx = 0;
  int back_idx = objs.size() - 1;

  // objs[front_idx] objs[back_idx]

  int mem_index = 0;
  bool finished = false;

  while (!finished) {
    if (objs[front_idx].id >= 0) {
      // front object is a file
      answer += CalculateProduct(mem_index, objs[front_idx].size, objs[front_idx].id);
      mem_index += objs[front_idx].size;
      ++front_idx;
    } else if (objs[front_idx].size >= objs[back_idx].size) {
      // Front object is empty space and bigger or equal to current back object
      answer += CalculateProduct(mem_index, objs[back_idx].size, objs[back_idx].id);
      mem_index += objs[back_idx].size;
      objs[front_idx].size -= objs[back_idx].size;
      back_idx -= 2; // Move to next file
    } else if (objs[front_idx].size < objs[back_idx].size) {
      // Front object is empty space and smaller than current back object
      answer += CalculateProduct(mem_index, objs[front_idx].size, objs[back_idx].id);
      mem_index += objs[front_idx].size;
      objs[back_idx].size -= objs[front_idx].size;
      objs[front_idx].size = 0;
    }

    // Move to next object if its current size is 0
    if (objs[front_idx].size == 0) {
      ++front_idx;
    }
    finished = mem_index >= size;
  }
  return answer;
}

long PartTwo(int size, std::vector<DiskObject> objs) {
  long answer = 0;
  // Remove last element if it is empty space
  if (objs.back().id == -1) {
    objs.pop_back();
  }

  // Iterate backwards over all files
  for (int i = objs.size() - 1; i >= 0; --i) {
    // Go to next file if on empty space
    if (objs[i].id == -1) {
      continue;
    }
    // Iterate forwards over all blank spaces
    for (int j = 1; j < i; ++j) {
      // Go to next space if on file
      if (objs[j].id != -1) {
        continue;
      }
      if (objs[j].size == objs[i].size) {
        objs[j].id = objs[i].id;
        objs[i].id = -1;  // Erase file at back
        break;
      } else if (objs[j].size > objs[i].size) {
        DiskObject remaining_space{objs[j].size - objs[i].size, -1};
        objs[j].id = objs[i].id;
        objs[j].size = objs[i].size;
        objs[i].id = -1;  // Erase file at back
        objs.insert(objs.begin() + j + 1, remaining_space);  // Fragment the empty space
        ++i;  // Account for insertion
        break;
      }
    }
  }

  int mem_index = 0;
  for (const DiskObject& obj : objs) {
    // Only add to answer if object is a file
    answer += CalculateProduct(mem_index, obj.size, obj.id) * (obj.id >= 0);
    mem_index += obj.size;
  }
  std::cout << std::endl;
  return answer;
}

int main() {
  auto start = std::chrono::steady_clock::now();
  Infoformat info = GetInfo();
  auto read_done = std::chrono::steady_clock::now();

  long answer_one = PartOne(info.first, info.second);
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
