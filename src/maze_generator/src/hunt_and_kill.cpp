#include "maze_generator/hunt_and_kill.hpp"
#include "maze_generator/cell.hpp"
#include "maze_generator/grid.hpp"
#include "maze_generator/rand_num_gen.hpp"
#include <unordered_map>
#include <vector>

void HuntAndKill::on(Grid &grid) {
  std::vector<Cell *> unvisited_vec;
  std::unordered_map<Cell *, size_t> unvisited_index;
  for (auto &row : grid.grid) {
    for (auto &cell : row) {
      unvisited_index[&cell] = unvisited_vec.size();
      unvisited_vec.push_back(&cell);
    }
  }
  auto remove_from_unvisited = [&](Cell *cell) {
    auto it = unvisited_index.find(cell);
    if (it == unvisited_index.end())
      return;

    size_t idx = it->second;
    Cell *last = unvisited_vec.back();

    unvisited_vec[idx] = last;
    unvisited_index[last] = idx;

    unvisited_vec.pop_back();
    unvisited_index.erase(cell);
  };
  Cell *current = grid.random_cell();
  remove_from_unvisited(current);
  while (current) {
    std::vector<Cell *> unvisited_neigbors;
    for (Cell *neighbor : current->neighbors()) {
      if (neighbor && neighbor->links().empty()) {
        unvisited_neigbors.push_back(neighbor);
      }
    }
    if (unvisited_neigbors.size() > 0) {
      Cell *neighbour = RandomNumberGenerator::sample(unvisited_neigbors);
      current->link(neighbour);
      remove_from_unvisited(neighbour);
      current = neighbour;
    } else {
      current = nullptr;
    }
    for (size_t i = 0; i < unvisited_vec.size(); ++i) {
      Cell *cell = unvisited_vec[i];
      std::vector<Cell *> visited_neighbors;
      for (Cell *neighbour : cell->neighbors()) {
        if (neighbour && !unvisited_index.count(neighbour)) {
          visited_neighbors.push_back(neighbour);
        }
      }
      if (visited_neighbors.size() > 0) {
        current = cell;
        Cell *neighbour = RandomNumberGenerator::sample(visited_neighbors);
        current->link(neighbour);
        remove_from_unvisited(current);
        break;
      }
    }
  }
}
