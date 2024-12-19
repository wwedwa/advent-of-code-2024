#include "utils.h"

using llong = long long;

struct Computer {

  void SetProgram(std::vector<int> new_program) {
    program = new_program;
  }

  void Reset() {
    A = 0;
    B = 0;
    C = 0;
    command_pointer = 0;
    output = "";
  }

  int GetComboOperand(int operand) {
    if (operand <= 3) {
      return operand;
    } else if (operand == 4) {
      return A;
    } else if (operand == 5) {
      return B;
    } else if (operand == 6) {
      return C;
    } else {
      return -1;
    }
  }

  void adv(int operand) {
    A = A >> operand;
  }

  void bxl(int operand) {
    B = B ^ operand;
  }

  void bst(int operand) {
    B = operand % 8;
  }

  void jnz(int operand) {
    if (A == 0) {
      return;
    }
    command_pointer = operand;
  }

  void bxc(int operand) {
    B = B ^ C;
  }

  void out(int operand) {
    // Separate by comma if output already has values
    if (output.size() > 0) {
      output += ",";
    }
    output += std::to_string(operand % 8);
  }

  void bdv(int operand) {
    B = A >> operand;
  }

  void cdv(int operand) {
    C = A >> operand;
  }

  void Run() {
    while (command_pointer < program.size()) {
      int opcode = program[command_pointer];
      int operand = program[command_pointer + 1];
      // If opcode requires combo operand, make sure operand isn't 7
      if ((opcode == 0 || opcode == 2 || opcode == 5 || opcode == 6 || opcode == 7) && operand == 7) {
        return;
      }
      // Call proper function and pass the operand
      if (opcode == 0) adv(GetComboOperand(operand));
      else if (opcode == 1) bxl(operand);
      else if (opcode == 2) bst(GetComboOperand(operand));
      else if (opcode == 3) jnz(operand);
      else if (opcode == 4) bxc(operand);
      else if (opcode == 5) out(GetComboOperand(operand));
      else if (opcode == 6) bdv(GetComboOperand(operand));
      else if (opcode == 7) cdv(GetComboOperand(operand));
      // Jump in all cases where jnz didn't already trigger a jump
      if (opcode != 3 || A == 0) {
        command_pointer += 2;
      }
    }
  }

  void Print() {
    std::cout << "Register A: " << A << std::endl;
    std::cout << "Register B: " << B << std::endl;
    std::cout << "Register C: " << C << std::endl;

    std::cout << "Program: ";
    for (int command : program) {
      std::cout << command << ",";
    }
    std::cout << std::endl;
    std::cout << "Output: " << output << std::endl;
  }

  int A;
  int B;
  int C;

  std::vector<int> program;
  int command_pointer;
  std::string output;

  std::unordered_map<int, std::function<void(int)>> command_map;
};


Computer GetInfo() {
  Computer computer;
  std::vector<int> program;
  std::ifstream input("input.txt");
  std::string line;
  // Process line for Register A
  std::getline(input, line);
  computer.A = aoc::GetInts(aoc::FindAllRegexStrings(line, "\\d+"))[0];
  // Process line for Register B
  std::getline(input, line);
  computer.B = aoc::GetInts(aoc::FindAllRegexStrings(line, "\\d+"))[0];
  // Process line for Register C
  std::getline(input, line);
  computer.C = aoc::GetInts(aoc::FindAllRegexStrings(line, "\\d+"))[0];
  // Get the program
  std::getline(input, line);  // Skip new line
  std::getline(input, line);
  computer.SetProgram(aoc::GetInts(aoc::FindAllRegexStrings(line, "\\d+")));
  
  return computer;
}

// This is specific for my input. But the same idea would work in general.
// Each loop through my program removes the last 3 bits of A until there are
// none left. Thus, we can work backwards and figure out which 3 bits to add
// to A in order to generate the expected output
void ReverseEngineer(llong a,
                    std::vector<int> curr_out,
                    const std::vector<int>& expected_out,
                    std::vector<llong>* a_options) {
  // Free up bottom 3 bits
  a = a << 3;
  // 3 bits has range 0 to 7
  for (int i = 0; i < 8; ++i) {
    llong new_a = a | i;  // Add i bits to a
    // Generate the b and c values as my input does
    llong b = i ^ 7;
    llong c = new_a >> b;
    b = (b ^ c ^ 4) % 8;
    if (b == expected_out[curr_out.size()]) {
      curr_out.push_back(b);
      // If we have generated the currect output, add a to all options that work
      // If not, continue building on a.
      if (curr_out.size() == expected_out.size()) {
        a_options->push_back(new_a);
      } else {
        ReverseEngineer(new_a, curr_out, expected_out, a_options);
      }
      // back track for other options
      curr_out.pop_back();
    }
  }
  // Only return once all paths have been explored
  return;
}

std::string PartOne(Computer computer) {
  computer.Run();
  return computer.output;
}

llong PartTwo(Computer computer) {
  std::vector<int> reverse_program;
  for (auto it = computer.program.rbegin(); it != computer.program.rend(); ++it) {
    reverse_program.push_back(*it);
  }
  std::vector<llong> a_options;
  ReverseEngineer(0, {}, reverse_program, &a_options);
  return *std::min_element(a_options.begin(), a_options.end());
}

int main() {
  auto start = std::chrono::steady_clock::now();
  Computer info = GetInfo();
  auto read_done = std::chrono::steady_clock::now();

  std::string answer_one = PartOne(info);
  auto part_one_done = std::chrono::steady_clock::now();

  llong answer_two = PartTwo(info);
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
