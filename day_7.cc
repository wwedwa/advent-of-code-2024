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
  Infoformat info = GetInfo();
  ulong answer_one = PartOne(info);
  ulong answer_two = PartTwo(info);
  std::cout << "Part One: " << answer_one << std::endl;
  std::cout << "Part Two: " << answer_two << std::endl;
  return 0;
}
