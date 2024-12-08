#include "utils.h"

using ulong = unsigned long;
// Each pair is the answer and the operands
using Infoformat = std::vector<std::pair<ulong, std::vector<ulong>>>;

Infoformat GetInfo() {
  Infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  while (std::getline(input, line)) {
    std::pair<ulong, std::vector<ulong>> equation;
    std::vector<std::string> eq = aoc::split(line, ": ");
    equation.first = std::stoul(eq[0]);
    equation.second = aoc::GetULongs(aoc::split(eq[1], " "));
    // We will treat this like a stack and do operations at the end
    // so reverse order
    std::reverse(equation.second.begin(), equation.second.end());
    info.push_back(equation);
  }
  return info;
}

bool IsPossible(const ulong& answer, const std::vector<ulong>& operands) {
  // First case shouldn't happen, but just to make sure...
  if (operands.empty()) {
    std::cout << "ERROR: Recursion down to 0" << std::endl;
    return false;
  } else if (operands.size() == 1) {
    return operands[0] == answer;
  }

  // If operands still remain, perform the mult. and add. on the first (last)
  // two elements
  std::vector<ulong> add_vec = operands;
  ulong add_operand = add_vec.back();
  add_vec.pop_back();
  add_vec[add_vec.size() - 1] += add_operand;

  std::vector<ulong> mult_vec = operands;
  ulong mult_operand = mult_vec.back();
  mult_vec.pop_back();
  mult_vec[mult_vec.size() - 1] *= mult_operand;

  return IsPossible(answer, add_vec) || IsPossible(answer, mult_vec);
}

bool IsPossibleWithConcat(const ulong& answer, const std::vector<ulong>& operands) {
  // First case shouldn't happen, but just to make sure...
  if (operands.empty()) {
    std::cout << "ERROR: Recursion down to 0" << std::endl;
    return false;
  } else if (operands.size() == 1) {
    return operands[0] == answer;
  }

  // If operands still remain, perform the mult., add., and concat
  //  on the first (last) two elements
  std::vector<ulong> add_vec = operands;
  ulong add_operand1 = add_vec.back();
  add_vec.pop_back();
  ulong add_operand2 = add_vec.back();
  add_vec.pop_back();
  add_vec.push_back(add_operand1 + add_operand2);

  std::vector<ulong> mult_vec = operands;
  ulong mult_operand1 = mult_vec.back();
  mult_vec.pop_back();
  ulong mult_operand2 = mult_vec.back();
  mult_vec.pop_back();
  mult_vec.push_back(mult_operand1 * mult_operand2);

  std::vector<ulong> concat_vec = operands;
  ulong concat_operand1 = concat_vec.back();
  concat_vec.pop_back();
  ulong concat_operand2 = concat_vec.back();
  concat_vec.pop_back();
  // Factor to scale operand by so that adding the two will be equivalent
  // to concatenating them
  ulong len_factor = std::pow(10, std::to_string(concat_operand2).size());
  concat_vec.push_back(concat_operand2 + (concat_operand1 * len_factor));

  return IsPossibleWithConcat(answer, add_vec) ||
        IsPossibleWithConcat(answer, mult_vec) ||
        IsPossibleWithConcat(answer, concat_vec);
}

ulong PartOne(Infoformat& info) {
  ulong answer = 0;
  for (const std::pair<ulong, std::vector<ulong>>& eq : info) {
    ulong eq_answer = eq.first;
    std::vector<ulong> eq_operands = eq.second;
    if (IsPossible(eq_answer, eq_operands)) {
      answer += eq_answer;
    }
  }
  return answer;
}

ulong PartTwo(Infoformat& info) {
  ulong answer = 0;
  for (const std::pair<ulong, std::vector<ulong>>& eq : info) {
    ulong eq_answer = eq.first;
    std::vector<ulong> eq_operands = eq.second;
    if (IsPossibleWithConcat(eq_answer, eq_operands)) {
      answer += eq_answer;
    }
  }
  return answer;
}

int main() {
  auto start = std::chrono::steady_clock::now();
  Infoformat info = GetInfo();
  auto read_done = std::chrono::steady_clock::now();

  ulong answer_one = PartOne(info);
  auto part_one_done = std::chrono::steady_clock::now();

  ulong answer_two = PartTwo(info);
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
