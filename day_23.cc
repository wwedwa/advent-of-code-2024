#include "utils.h"

using Graph = std::unordered_map<std::string, std::unordered_set<std::string>>;
using Infoformat = std::pair<Graph, std::vector<std::string>>;

Infoformat GetInfo() {
  Infoformat info;
  std::ifstream input("input.txt");
  std::string line;
  while (std::getline(input, line)) {
    std::vector<std::string> connection = aoc::split(line, "-");
    info.first[connection[0]].insert(connection[1]);
    info.first[connection[1]].insert(connection[0]);
    // This is really slow, but vector is easier to use in the actual problem
    if (std::find(info.second.begin(), info.second.end(), connection[0]) == info.second.end())
      info.second.push_back(connection[0]);
    if (std::find(info.second.begin(), info.second.end(), connection[1]) == info.second.end())
      info.second.push_back(connection[1]);
  }
  return info;
}

bool IsClique(const std::vector<std::string>& subgraph, const Graph& graph) {
  for (int i = 0; i < subgraph.size() - 1; ++i) {
    for (int j = i + 1; j < subgraph.size(); ++j) {
      if (!graph.at(subgraph[i]).count(subgraph[j])) {
        return false;
      }
    }
  }
  return true;
}

bool ContainsTNeighbor(std::unordered_set<std::string> neighbors) {
  for (const std::string& neighbor : neighbors) {
    if (neighbor[0] == 't') {
      return true;
    }
  }
  return false;
}

bool CliqueContainsT(const std::vector<std::string>& clique) {
  for (const std::string& node : clique) {
    if (node[0] == 't') {
      return true;
    }
  }
  return false;
}

void FindCliques(const Graph& graph, const std::vector<std::string>& nodes, std::vector<std::string> clique, int curr_node, int* count) {
  if (clique.size() == 3) {
    *count += CliqueContainsT(clique);
    return;
  }

  for (int i = curr_node; i < nodes.size(); ++i) {
    if (nodes[i][0] != 't' && !ContainsTNeighbor(graph.at(nodes[i]))) {
      continue;
    }
    clique.push_back(nodes[i]);
    if (IsClique(clique, graph)) {
      FindCliques(graph, nodes, clique, i + 1, count);
    }
    clique.pop_back();
  }
}


// Implementing a version of Bron–Kerbosch (https://en.wikipedia.org/wiki/Bron–Kerbosch_algorithm)
// This is the recursive part of it
void _FindMaximumClique(std::unordered_set<std::string> R, std::unordered_set<std::string> P, std::unordered_set<std::string> X, const Graph& graph, std::unordered_set<std::string>* max_clique) {
  if (P.empty() && X.empty() && R.size() > max_clique->size()) {
    *max_clique = R;
    return;
  }

  for (auto it = P.begin(); it != P.end(); it = P.erase(it)) {
    std::string node = *it;
    R.insert(node);

    std::unordered_set<std::string> new_P;
    std::unordered_set<std::string> new_X;
    // New X and P are themselves intersected with neighbors of the node
    for (const std::string& neighbor : graph.at(node)) {
      if (P.count(neighbor)) {
        new_P.insert(neighbor);
      }
      if (X.count(neighbor)) {
        new_X.insert(neighbor);
      }
    }

    _FindMaximumClique(R, new_P, new_X, graph, max_clique);

    R.erase(node);
    X.insert(node);
  }
}

std::unordered_set<std::string> FindMaximumClique(const Graph& graph, const std::vector<std::string>& nodes) {
  std::unordered_set<std::string> node_set(nodes.begin(), nodes.end());
  std::unordered_set<std::string> max_clique;
  _FindMaximumClique({}, node_set, {}, graph, &max_clique);
  return max_clique;
}

int PartOne(const Graph& graph, const std::vector<std::string>& nodes) {
  int answer = 0;
  FindCliques(graph, nodes, {}, 0, &answer);
  return answer;
}

std::string PartTwo(const Graph& graph, const std::vector<std::string>& nodes) {
  std::string answer;
  std::unordered_set<std::string> max_clique = FindMaximumClique(graph, nodes);
  std::set<std::string> ordered_clique(max_clique.begin(), max_clique.end());
  for (const std::string& node : ordered_clique) {
    if (answer.size() > 0) {
      answer += ',';
    }
    answer += node;
  }
  return answer;
}

int main() {
  auto start = std::chrono::steady_clock::now();
  Infoformat info = GetInfo();
  auto read_done = std::chrono::steady_clock::now();

  int answer_one = PartOne(info.first, info.second);
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
