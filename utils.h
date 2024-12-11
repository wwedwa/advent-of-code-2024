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
#include <stack>
#include <list>
#include <utility>
#include <tuple>
#include <algorithm>
#include <climits>
#include <numeric>
#include <regex>
#include <functional>

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

/**
 * @brief A generic BFS algorithm that allows optional processing of the current
 * path
 * 
 * Nodes must be uniquely identifiable so that BFS can distinguish between
 * nodes that have been visited and nodes that have not. Also, NodeType must
 * define < and == for use in std::find and sets/maps
 * 
 * @param start_node The node to start the search at
 * @param GetNeighbors A function that returns the neighbors of the current node
 * @param ProcessPath An optional function that takes the current path of nodes 
 * for any neccessary processing. Called every time BFS moves to a new node.
 * @param EndCondition An optional function that is called every time BFS moves 
 * to a new node. If true, the search ends. Default is to always return false.
 * It is called after ProcessNode
 */
template <typename NodeType>
void BFS(
  const NodeType& start_node,
  const std::function<std::vector<NodeType>(const NodeType&)>& GetNeighbors,
  const std::function<void(const std::vector<NodeType>&)>& ProcessPath = [](const std::vector<NodeType>&) {},
  const std::function<bool(const std::vector<NodeType>&)>& EndCondition = [](const std::vector<NodeType>&) { return false; }
) {
  std::queue<NodeType> node_queue;  // Current queue of nodes to visit
  std::set<NodeType> visited;  // Nodes that have already been visited
  std::vector<NodeType> path;  // Current path that BFS is on
  std::map<NodeType, int> node_depth;  // Track depth of all nodes for constructing path
  node_queue.push(start_node);
  visited.insert(start_node);
  path.push_back(start_node);
  node_depth[start_node] = 0;

  int curr_depth = 0;
  while(!node_queue.empty()) {
    // Get and remove next node from queue
    NodeType curr_node = node_queue.front();
    node_queue.pop();
    // Update current path
    if (curr_depth == node_depth[curr_node]) {
      // If curr_node is on same level as last node in path, replace last node
      path.pop_back();
    }
    path.push_back(curr_node);
    curr_depth = node_depth[curr_node];  // Update current depth

    // Now process the updated path
    ProcessPath(path);
    // Check if BFS should end
    if (EndCondition(path)) {
      return;
    }
    
    for (const NodeType& neighbor : GetNeighbors(curr_node)) {
      if (!visited.count(neighbor)) {
        node_queue.push(neighbor);
        visited.insert(neighbor);
        node_depth[neighbor] = node_depth[curr_node] + 1;
      }
    }
  }
  return;
}

/**
 * @brief A generic DFS algorithm that allows optional processing of the current
 * path
 * 
 * Nodes must be uniquely identifiable so that DFS can distinguish between
 * nodes that have been visited and nodes that have not. Also, NodeType must
 * define < and == for use in std::find and sets/maps
 * 
 * @param start_node The node to start the search at
 * @param GetNeighbors A function that returns the neighbors of the current node
 * @param ProcessPath An optional function that takes the current path of nodes 
 * for any neccessary processing. Called every time BFS moves to a new node.
 * @param EndCondition An optional function that is called every time BFS moves 
 * to a new node. If true, the search ends. Default is to always return false.
 * It is called after ProcessNode
 * @param revisit_nodes A flag for if the DFS algorithm should ever revisit nodes.
 * Note that the algorithm will NEVER revisit nodes that are on the current path
 * to avoid cycles. However, when in a different path, it will visit already
 * visited nodes only if revisit_nodes is true. Defaults to false. This can be
 * used to walk all non-cyclic paths starting from start_node
 */
template <typename NodeType>
void DFS(
  const NodeType& start_node,
  const std::function<std::vector<NodeType>(const NodeType&)>& GetNeighbors,
  const std::function<void(const std::vector<NodeType>&, int)>& ProcessPath = [](const std::vector<NodeType>&) {},
  const std::function<bool(const std::vector<NodeType>&)>& EndCondition = [](const std::vector<NodeType>&) { return false; },
  bool revisit_nodes = false
) {
  std::stack<NodeType> node_stack;  // Current stack of nodes to visit
  std::set<NodeType> visited;  // Nodes that have already been visited
  std::vector<NodeType> path;  // Current path that DFS is on
  std::map<NodeType, int> node_depth;  // Track depth of all nodes for constructing path
  node_stack.push(start_node);
  visited.insert(start_node);
  path.push_back(start_node);
  node_depth[start_node] = 0;

  int curr_depth = 0;

  while (!node_stack.empty()) {
    NodeType curr_node = node_stack.top();
    node_stack.pop();

    // Update current path
    while (curr_depth >= node_depth[curr_node]) {
      --curr_depth;
      path.pop_back();
    }
    path.push_back(curr_node);
    curr_depth = node_depth[curr_node];  // Update current depth

    // Now process the updated path
    ProcessPath(path, curr_depth);
    // Check if DFS should end
    if (EndCondition(path)) {
      return;
    }
    // Only add neighbors if it hasn't been visited (if revisit_nodes is false)
    // or if it has been visited in the current path
    if (!visited.count(curr_node) || revisit_nodes) {
      visited.insert(curr_node);
      for (const NodeType& neighbor : GetNeighbors(curr_node)) {
        if (std::find(path.begin(), path.end(), neighbor) == path.end()) {
          node_depth[neighbor] = node_depth[curr_node] + 1;
          node_stack.push(neighbor);
        }
      }
    }
  }
}


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