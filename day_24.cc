#include "utils.h"

using ullong = unsigned long long;
using WireVals = std::unordered_map<std::string, bool>;
using Gates = std::unordered_map<std::string, std::vector<std::string>>;
using Infoformat = std::pair<WireVals, Gates>;

Infoformat GetInfo() {
  Infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  // Get initial wire values
  while (std::getline(input, line) && !line.empty()) {
    std::vector<std::string> gate = aoc::split(line, ": ");
    info.first[gate[0]] = (gate[1] == "1");
  }

  // Get gates
  while (std::getline(input, line)) {
    std::vector<std::string> in_out = aoc::split(line, " -> ");
    std::vector<std::string> gate = aoc::split(in_out[0], " ");
    info.second[in_out[1]] = gate;
  }
  return info;
}

ullong PartOne(WireVals wires, const Gates& gates) {
  ullong answer = 0;
  bool done = false;
  while (!done) {
    done = true;
    for (auto [out, gate] : gates) {
      if (!wires.count(gate[0]) || !wires.count(gate[2])) {
        done = (out[0] == 'z') ? false : done;  // If one of the inputs is unknown and the resulting wire
                                                // is one of the z wires, we are not done
        continue;
      }
      if (gate[1] == "AND") wires[out] = wires[gate[0]] && wires[gate[2]];
      else if (gate[1] == "OR") wires[out] = wires[gate[0]] || wires[gate[2]];
      else if (gate[1] == "XOR") wires[out] = wires[gate[0]] ^ wires[gate[2]];
    }
  }

  for (auto [wire, val] : wires) {
    if (wire[0] != 'z') {
      continue;
    }
    int bit_num = std::stoi(wire.substr(1));
    ullong bit_val = (ullong)val << bit_num;
    answer |= bit_val;
  }
  return answer;
}

// Given a wire and gate type, it'll find the output wire if such a gate exists
// If none exists, return empty string
std::string FindMatchingGate(const Gates& gates, std::string wire, std::string gate_type) {
  for (auto [key, val] : gates) {
    if (val[1] == gate_type && (val[0] == wire || val[2] == wire)) {
      return key;
    }
  }
  return "";
}

// The circuit should be a ripple carry adder. So, just check if each input
// goes to the proper gates, and each gate output also goes to the proper gates.
// If there are gates which don't, return their names.
// THIS METHOD IS NOT GUARANTEED TO WORK FOR ANY GENERAL OUTPUT. But it works for mine
// I just kept adding checks until I got 8 wrong wires
// Reference diagram: https://www.build-electronic-circuits.com/full-adder/
std::vector<std::string> FindWrongGates(const Gates& gates, std::string x_wire) {
  std::vector<std::string> wrong_gates;
  // We can always find these since it's only the outputs which are swapped
  std::string xor_gate1 = FindMatchingGate(gates, x_wire, "XOR");
  std::string and_gate1 = FindMatchingGate(gates, x_wire, "AND");

  // In a full adder curcuit, the output of the first XOR gate goes to a
  // second XOR and an AND gate. The second XOR gate will go to an output bit
  std::string xor_gate2 = FindMatchingGate(gates, xor_gate1, "XOR");
  std::string and_gate2 = FindMatchingGate(gates, xor_gate1, "AND");

  // If the first XOR gate does NOT go to another XOR gate and an AND gate,
  // then this first XOR gate is wrong
  if (xor_gate2 == "" || 
      and_gate2 == "") {
    wrong_gates.push_back(xor_gate1);
  }

  // xor_gate2 should be an output bit and it's bit number should match
  // the input's bit number
  if (xor_gate2 != "" && xor_gate2.substr(1) != x_wire.substr(1)) {
    wrong_gates.push_back(xor_gate2);
  }

  // and_gate2 should NOT be an output bit
  if (and_gate2 != "" && and_gate2[0] == 'z') {
    wrong_gates.push_back(and_gate2);
  }

  // In a full adder curcuit, the output of the first AND gate goes to an OR gate.
  std::string or_gate1 = FindMatchingGate(gates, and_gate1, "OR");
  if (or_gate1 == "") {
    wrong_gates.push_back(and_gate1);
  } else {
    // The OR gate then should go to an XOR gate and an AND gate
    std::string xor_gate3 = FindMatchingGate(gates, or_gate1, "XOR");
    std::string and_gate3 = FindMatchingGate(gates, or_gate1, "AND");
    if (xor_gate3 == "" ||
        and_gate3 == "") {
      wrong_gates.push_back(or_gate1);
    }
  }
  return wrong_gates;
}

std::string PartTwo(const WireVals& info, const Gates& gates) {
  std::string answer;
  std::set<std::string> ordered_wrong_wires;
  for (int i = 1; i < 44; ++i) {
    std::string x_wire = "x";
    if (i < 10) {
      x_wire += "0";
    }
    x_wire += std::to_string(i);
    std::vector<std::string> wrong_wires = FindWrongGates(gates, x_wire);
    for (const std::string& wire : wrong_wires) {
      ordered_wrong_wires.insert(wire);
    }
  }
  
  for (const std::string& wire : ordered_wrong_wires) {
    if (answer.size() > 0) {
      answer += ",";
    }
    answer += wire;
  }
  return answer;
}

int main() {
  auto start = std::chrono::steady_clock::now();
  Infoformat info = GetInfo();
  auto read_done = std::chrono::steady_clock::now();

  ullong answer_one = PartOne(info.first, info.second);
  auto part_one_done = std::chrono::steady_clock::now();

  std::string answer_two = PartTwo(info.first, info.second);
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
