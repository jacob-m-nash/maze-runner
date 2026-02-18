#include "maze_generator/binary_tree.hpp"
#include "maze_generator/rand_num_gen.hpp"
void BinaryTree::on(Grid &grid) {
  for (auto &row : grid.grid) {
    for (auto &cell : row) {
      std::vector<Cell *> neighbours;
      if (cell.north) {
        neighbours.push_back(cell.north);
      }
      if (cell.east) {
        neighbours.push_back(cell.east);
      }
      if (!neighbours.empty()) {
        int index = RandomNumberGenerator::random_int(0, neighbours.size() - 1);
        Cell *neighbour = neighbours[index];
        cell.link(neighbour);
      }
    }
  }
}
