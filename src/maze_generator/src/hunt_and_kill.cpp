#include "maze_generator/hunt_and_kill.hpp"
#include "maze_generator/cell.hpp"
#include "maze_generator/grid.hpp"
#include "maze_generator/rand_num_gen.hpp"
#include <vector>

void HuntAndKill::on(Grid &grid) {
  Cell *current = grid.random_cell();
  while (current) {
    std::vector<Cell *> unvisited_neigbors;
    for (Cell *neighbor : current->neighbors()) {
      if (neighbor && neighbor->links().empty()) {
        unvisited_neigbors.push_back(neighbor);
      }
    }
    if (unvisited_neigbors.size() > 0) {
      int index =
          RandomNumberGenerator::random_int(0, unvisited_neigbors.size() - 1);
      Cell *neighbour = unvisited_neigbors[index];
      current->link(neighbour);
      current = neighbour;
    } else {
      current = nullptr;
    }
    for (auto &row : grid.grid) {
      for (auto &cell : row) {
        std::vector<Cell *> visited_neigbors;
        for (Cell *neighbor : cell.neighbors()) {
          if (!neighbor->links().empty()) {
            visited_neigbors.push_back(neighbor);
          }
          if (cell.links().empty() && visited_neigbors.size() > 0) {
            current = &cell;

            int index = RandomNumberGenerator::random_int(
                0, visited_neigbors.size() - 1);
            Cell *neighbour = visited_neigbors[index];
            current->link(neighbour);
          }
        }
      }
    }
  }
}
