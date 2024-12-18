#ifndef UTILS_H
#define UTILS_H

// All of this is included just so I don't have to include it separately in every day
#include <iostream>
#include <chrono>
#include <stdlib.h>
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

// Gets all unsigned long longs from a vector of strings
std::vector<unsigned long long> GetULLongs(const std::vector<std::string>& strings);

// Calculate gcd with Euclid's Algorithm
long gcd(long a, long b);

// Calculate lcm for a vector of ints
long lcm(std::vector<int> nums);

// Calculate a special mod
int PosMod(int m, int n);

// Find all instances of a regex pattern in the given string. Returned as strings
std::vector<std::string> FindAllRegexStrings(std::string input, std::string pattern);

// Find all instances of a regex pattern in the given string. Returned as smatch
// object for post processing.
std::vector<std::smatch> FindAllRegexMatches(std::string input, std::string pattern);

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
 * @param revisit_nodes A flag for if the BFS algorithm should ever revisit nodes.
 * Note that the algorithm will NEVER revisit nodes that are on the current path
 * to avoid cycles. However, when in a different path, it will visit already
 * visited nodes only if revisit_nodes is true. Defaults to false. This can be
 * used to walk all non-cyclic paths starting from start_node
 */
template <typename NodeType>
void BFS(
  const NodeType& start_node,
  const std::function<std::vector<NodeType>(const std::vector<NodeType>&)>& GetNeighbors,
  const std::function<void(const std::vector<NodeType>&)>& ProcessPath = [](const std::vector<NodeType>&) {},
  const std::function<bool(const std::vector<NodeType>&)>& EndCondition = [](const std::vector<NodeType>&) { return false; },
  bool revisit_nodes = false
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
    
    for (const NodeType& neighbor : GetNeighbors(path)) {
      if ((!visited.count(neighbor) || revisit_nodes) && std::find(path.begin(), path.end(), neighbor) == path.end()) {
        node_queue.push(neighbor);
        if (!revisit_nodes) {
          visited.insert(neighbor);
        }
        node_depth[neighbor] = node_depth[curr_node] + 1;
      }
    }
  }
  return;
}

template <typename NodeType>
struct DFSState {

};

// The recursive function called from DFS. Call DFS, not this function
template <typename NodeType>
void DFSRecursive(
  const NodeType& node,
  const std::function<std::vector<NodeType>(const std::vector<NodeType>&)>& GetNeighbors,
  const std::function<void(const std::vector<NodeType>&)>& ProcessPath,
  const std::function<bool(const std::vector<NodeType>&)>& EndCondition,
  bool revisit_nodes,
  bool allow_cycles,
  int depth,
  std::set<NodeType>& visited,
  std::vector<NodeType>& path
) {
  visited.insert(node);
  path.push_back(node);
  ProcessPath(path);

  if (EndCondition(path)) {
    path.pop_back();
    return;
  }
  
  for (const NodeType& neighbor : GetNeighbors(path)) {
    bool add_node = (!visited.count(neighbor) || revisit_nodes) &&
                    (std::find(path.begin(), path.end(), neighbor) == path.end() ||
                    allow_cycles) &&
                    (!allow_cycles || path.size() <= depth);
    if (add_node) {
      DFSRecursive<NodeType>(neighbor, GetNeighbors, ProcessPath,
                            EndCondition, revisit_nodes, allow_cycles,
                            depth, visited, path);
    }
  }
  path.pop_back();
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
 * @param ProcessPath A function that takes the current path of nodes 
 * for any neccessary processing. Called every time DFS moves to a new node,
 * before checking out its neighbors.
 * @param EndCondition An optional function that is called every time BFS moves 
 * to a new node. If true, the search ON THE CURRENT NODE ends. To end the
 * entire search, handle that in your implemenation of EndCondition.
 * @param revisit_nodes A flag for if the DFS algorithm should ever revisit
 * nodes that it visited in previous paths but NOT in the current path. Default
 * is false. Good for searching all paths in a tree
 * @param allow_cycles A flag for if DFS algorithm should allow cycles. If true,
 * revisit_nodes is automatically set to true. Default is false.
 * @param depth An int only used if allow_cycles is true. Will stop searching
 * down a path when depth is reached. MUST BE SET IF ALLOWING CYCLES
 */
template <typename NodeType>
void DFS(
  const NodeType& start_node,
  const std::function<std::vector<NodeType>(const std::vector<NodeType>&)>& GetNeighbors,
  const std::function<void(const std::vector<NodeType>&)>& ProcessPath = [](const std::vector<NodeType>&) {},
  const std::function<bool(const std::vector<NodeType>&)>& EndCondition = [](const std::vector<NodeType>&) { return false; },
  bool revisit_nodes = false,
  bool allow_cycles = false,
  int depth = -1
) {
  std::set<NodeType> visited;
  std::vector<NodeType> path;
  // Start recursive calls with empty visited set and path vector
  DFSRecursive<NodeType>(start_node, GetNeighbors, ProcessPath,
                        EndCondition, revisit_nodes, allow_cycles,
                        depth, visited, path);
  return;
}

// Comparator used in the priority queue in Dijkstras. Elements with shortest
// distance from start node have greater priority.
template <typename NodeType>
struct Compare {
  bool operator()(const std::pair<int, NodeType>& a,
                  const std::pair<int, NodeType>& b) const {
    return a.first > b.first;
  }
};

// Struct to maintain the state of Dijkstra's algo
template <typename NodeType>
struct DijkstraState {
  NodeType curr_node;
  std::map<NodeType, NodeType> prev_nodes;
  std::map<NodeType, int> dists;
  std::priority_queue<std::pair<int, NodeType>,
                      std::vector<std::pair<int, NodeType>>,
                      Compare<NodeType>> to_visit;
};

/**
 * @brief A generic Dijkstra's algorithm that allows optional processing of
 * the current node and path
 * 
 * NodeType must define < and == for use in maps. Each function argument
 * is given the entire state of the algorithm: the current node, a map of all
 * nodes and the previous nodes travelled to get to them (for path reconstructon),
 * and the current shortest distances from each node to the start
 * 
 * @param start_node The node to start the search at
 * @param GetNeighbors A function that returns neighbors and the weights of the
 * edges which connect them to the current node
 * @param ProcessPath A function that takes the state of the algo for any
 * neccessary processing. Called every time the algo moves to a new node
 * @param EndCondition An optional function that is called every time the algo
 * moves to a new node. If true, the search ends.
 */
template <typename NodeType>
DijkstraState<NodeType> Dijkstra(
  const NodeType& start_node,
  const std::function<std::map<NodeType, int>(const DijkstraState<NodeType>& state)>& GetNeighbors,
  const std::function<void(const DijkstraState<NodeType>& state)>& ProcessPath = [](const DijkstraState<NodeType>& state) {},
  const std::function<bool(const DijkstraState<NodeType>& state)>& EndCondition = [](const DijkstraState<NodeType>& state) { return false; }
) {

  DijkstraState<NodeType> state;
  auto& prev_nodes = state.prev_nodes;
  auto& dists = state.dists;
  auto& to_visit = state.to_visit;
  
  prev_nodes[start_node] = start_node;
  dists[start_node] = 0;
  to_visit.emplace(0, start_node);

  while (!to_visit.empty()) {
    auto [start_dist, curr_node] = to_visit.top();
    state.curr_node = curr_node;
    to_visit.pop();

    ProcessPath(state);
    
    if (EndCondition(state)) {
      return state;
    }

    for (const auto& [neighbor, edge_dist] : GetNeighbors(state)) {
      // Add neighbor if it is not in dists map (aka distance of infinity)
      // or if new calculated dist is less than the dist in the dists map.
      int new_dist = start_dist + edge_dist;
      if (!dists.count(neighbor) || new_dist < dists[neighbor]) {
        dists[neighbor] = new_dist;
        prev_nodes[neighbor] = curr_node;
        to_visit.emplace(new_dist, neighbor);
      }
    }
  }
  return state;
}

// Struct to maintain the state of Dijkstra's algo
template <typename NodeType>
struct ExDijkstraState {
  NodeType curr_node;
  // Save all prev nodes that lead to a path of equivalent distance from start
  std::map<NodeType, std::set<NodeType>> prev_nodes;
  std::map<NodeType, int> dists;
  std::priority_queue<std::pair<int, NodeType>,
                      std::vector<std::pair<int, NodeType>>,
                      Compare<NodeType>> to_visit;
};

/**
 * @brief An extended Dijkstra's algorithm for finding ALL shortest paths from
 * the start to any node. The returned state will hold prev_nodes, a subgraph
 * containing all of these shortest paths.
 * 
 * NodeType must define < and == for use in maps. Each function argument
 * is given the entire state of the algorithm: the current node, a map of all
 * nodes and the previous nodes travelled to get to them (for path reconstructon),
 * and the current shortest distances from each node to the start
 * 
 * @param start_node The node to start the search at
 * @param GetNeighbors A function that returns neighbors and the weights of the
 * edges which connect them to the current node
 * @param ProcessPath A function that takes the state of the algo for any
 * neccessary processing. Called every time the algo moves to a new node
 * @param EndCondition An optional function that is called every time the algo
 * moves to a new node. If true, the search ends.
 */
template <typename NodeType>
ExDijkstraState<NodeType> ExtendedDijkstra(
  const NodeType& start_node,
  const std::function<std::map<NodeType, int>(const ExDijkstraState<NodeType>& state)>& GetNeighbors,
  const std::function<void(const ExDijkstraState<NodeType>& state)>& ProcessPath = [](const ExDijkstraState<NodeType>& state) {},
  const std::function<bool(const ExDijkstraState<NodeType>& state)>& EndCondition = [](const ExDijkstraState<NodeType>& state) { return false; }
) {

  ExDijkstraState<NodeType> state;
  state.curr_node = start_node;
  auto& prev_nodes = state.prev_nodes;
  auto& dists = state.dists;
  auto& to_visit = state.to_visit;
  
  prev_nodes[start_node].insert(start_node);
  dists[start_node] = 0;
  to_visit.emplace(0, start_node);

  while (!to_visit.empty()) {
    auto [start_dist, curr_node] = to_visit.top();
    state.curr_node = curr_node;
    to_visit.pop();

    ProcessPath(state);
    
    if (EndCondition(state)) {
      return state;
    }

    for (const auto& [neighbor, edge_dist] : GetNeighbors(state)) {
      // Add neighbor if it is not in dists map (aka distance of infinity)
      // or if new calculated dist is less than the dist in the dists map.
      int new_dist = start_dist + edge_dist;
      if (!dists.count(neighbor) || new_dist < dists[neighbor]) {
        dists[neighbor] = new_dist;
        prev_nodes[neighbor] = {curr_node};
        to_visit.emplace(new_dist, neighbor);
      } else if (new_dist == dists[neighbor]) {
        prev_nodes[neighbor].insert(curr_node);
      }
    }
  }
  return state;
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