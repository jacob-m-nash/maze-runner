#include "maze_generator/wilsons.hpp"
#include "maze_generator/rand_num_gen.hpp"
#include <algorithm>
#include <cstddef>
#include <unordered_map>
#include <vector>
void Wilsons::on(Grid &grid) {
  std::vector<Cell *> unvisited_vec;
  std::unordered_map<Cell *, size_t> unvisited_index;
  for (auto &row : grid.grid) { // Set up unvisited
    for (auto &cell : row) {
      unvisited_index[&cell] = unvisited_vec.size();
      unvisited_vec.push_back(&cell);
    }
  }
  auto remove_from_unvisited = [&](Cell *cell) {
    auto it = unvisited_index.find(cell);
    if (it == unvisited_index.end())
      return;
    size_t index = it->second;
    Cell *last = unvisited_vec.back();
    unvisited_vec[index] = last;
    unvisited_index[last] = index;

    unvisited_vec.pop_back();
    unvisited_index.erase(cell);
  };
  // Walk and carve until we have visited all cells
  Cell *first = RandomNumberGenerator::sample(unvisited_vec);
  remove_from_unvisited(first);
  while (unvisited_vec.size() > 0) {
    Cell *cell = RandomNumberGenerator::sample(unvisited_vec);
    std::vector<Cell *> path;
    path.push_back(cell);
    while (unvisited_index.count(cell)) {
      cell = RandomNumberGenerator::sample(cell->neighbors());
      // if this next cell is in path we have a loop
      auto it = std::find(path.begin(), path.end(), cell);
      if (it != path.end()) {
        // remove the cells that make the loop (everything after the already
        // visited cell)
        path.erase(it + 1, path.end());
      } else {
        path.push_back(cell);
      }
    }
    for (size_t i = 0; i + 1 < path.size(); ++i) {
      path[i]->link(path[i + 1]);
      remove_from_unvisited(path[i]);
    }
  }
}
