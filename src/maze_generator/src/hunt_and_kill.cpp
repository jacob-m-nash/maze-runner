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
    std::vector<Cell *> unvisited_neighbours;
    for (Cell *neighbour : current->neighbours()) {
      if (neighbour && neighbour->links().empty()) {
        unvisited_neighbours.push_back(neighbour);
      }
    }
    if (unvisited_neighbours.size() > 0) {
      Cell *neighbour = RandomNumberGenerator::sample(unvisited_neighbours);
      current->link(neighbour);
      remove_from_unvisited(neighbour);
      current = neighbour;
    } else {
      current = nullptr;
    }
    for (size_t i = 0; i < unvisited_vec.size(); ++i) {
      Cell *cell = unvisited_vec[i];
      std::vector<Cell *> visited_neighbours;
      for (Cell *neighbour : cell->neighbours()) {
        if (neighbour && !unvisited_index.count(neighbour)) {
          visited_neighbours.push_back(neighbour);
        }
      }
      if (visited_neighbours.size() > 0) {
        current = cell;
        Cell *neighbour = RandomNumberGenerator::sample(visited_neighbours);
        current->link(neighbour);
        remove_from_unvisited(current);
        break;
      }
    }
  }
}
