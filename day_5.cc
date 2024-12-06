#include "utils.h"

struct Page {

  bool operator< (const Page& page) const {
    return std::find(greater_than.begin(),
                  greater_than.end(),
                  page.val) != greater_than.end();
  }

  bool operator== (const Page& page) const {
    return val == page.val;
  }

  int val;
  // All the values that this Page is less than
  std::vector<int> less_than;
  // All the values that this Page is greater than
  std::vector<int> greater_than;
};

typedef std::vector<std::vector<Page>> Infoformat;

Infoformat GetInfo() {
  // Keep track of pages as we make them
  std::map<int, Page> pages;

  Infoformat info;

  std::ifstream input("input.txt");
  std::string line;
  // First get rules
  while (std::getline(input, line) && !line.empty()) {
    std::vector<int> rule = aoc::GetInts(aoc::split(line, "|"));
    // Handle the page in the first position of the rule
    if (!pages.count(rule[0])) {
      Page page;
      page.val = rule[0];
      page.greater_than.push_back(rule[1]);
      pages[rule[0]] = page;
    } else {
      pages[rule[0]].greater_than.push_back(rule[1]);
    }

    // Handle the page in the second position of the rule
    if (!pages.count(rule[1])) {
      Page page;
      page.val = rule[1];
      page.less_than.push_back(rule[0]);
      pages[rule[1]] = page;
    } else {
      pages[rule[1]].less_than.push_back(rule[0]);
    }
  }

  // Then get updates
  while (std::getline(input, line)) {
    std::vector<int> update = aoc::GetInts(aoc::split(line, ","));
    std::vector<Page> update_pages;
    for (int val : update) {
      update_pages.push_back(pages[val]);
    }
    info.push_back(update_pages);
  }

  return info;
}

// Check if pages are in order. Page rules are not necessarily transitive
// so have to compare all elements to all elements ahead of it
bool CheckUpdate(const std::vector<Page>& update) {
  for (int i = 0; i < update.size() - 1; ++i) {
    for (int j = i + 1; j < update.size(); ++j) {
      if (!(update[i] < update[j])) {
        return false;
      }
    }
  }
  return true;
}

int PartOne(Infoformat& info) {
  int answer = 0;

  for (std::vector<Page> update : info) {
    if (CheckUpdate(update)) {
      answer += update[update.size()/2].val;
    }
  }

  return answer;
}

int PartTwo(Infoformat& info) {
  int answer = 0;

  for (std::vector<Page> update : info) {
    if (!CheckUpdate(update)) {
      // This technically assumes the rules are transitive (which they aren't
      // required to be). However, it should still work since the answer needs
      // to be unique. So even if the rules aren't transitive with respect to
      // all page values, they are transitive with respect to the page numbers
      // in any given update. Moreover, we can assume there are enough rules to
      // provide a unique ordering of any update
      // (or else it would be impossible to sort uniquely or sort at all)
      std::sort(update.begin(), update.end());
      answer += update[update.size()/2].val;
    }
  }

  return answer;
}

int main() {
  Infoformat info = GetInfo();
  int answer_one = PartOne(info);
  int answer_two = PartTwo(info);
  std::cout << "Part One: " << answer_one << std::endl;
  std::cout << "Part Two: " << answer_two << std::endl;
  return 0;
}
