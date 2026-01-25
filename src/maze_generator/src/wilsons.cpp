#include "maze_generator/wilsons.hpp"
#include "maze_generator/rand_num_gen.hpp"
#include <algorithm>
#include <iterator>
#include <unordered_set>
#include <vector>
void Wilsons::on(Grid &grid) {
  std::unordered_set<Cell *> unvisited;
  for (auto &row : grid.grid) {
    for (auto &cell : row) {
      unvisited.insert(&cell);
    }
  }
  int index = RandomNumberGenerator::random_int(0, unvisited.size() - 1);
  auto it = unvisited.begin();
  std::advance(it, index);
  Cell *first = *it;
  unvisited.erase(first);
  while (unvisited.size() > 0) {
    index = RandomNumberGenerator::random_int(0, unvisited.size() - 1);
    it = unvisited.begin();
    std::advance(it, index);
    Cell *cell = *it;
    std::vector<Cell *> path;
    path.push_back(cell);
    while (unvisited.find(cell) != unvisited.end()) {
      int index =
          RandomNumberGenerator::random_int(0, cell->neighbors().size() - 1);
      cell = cell->neighbors()[index];
      auto it = std::find(path.begin(), path.end(), cell);
      if (it != path.end()) {
        path.erase(it + 1, path.end());
      } else {
        path.push_back(cell);
      }
    }
    for (size_t i = 0; i + 1 < path.size(); ++i) {
      path[i]->link(path[i + 1]);
      unvisited.erase(path[i]);
    }
  }
}
