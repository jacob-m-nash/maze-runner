#include "maze_generator/binary_tree.hpp"
#include "maze_generator/rand_num_gen.hpp"
void BinaryTree::on(Grid &grid) {
  for (auto &row : grid.grid) {
    for (auto &cell : row) {
      std::vector<Cell *> neigbours;
      if (cell.north) {
        neigbours.push_back(cell.north);
      }
      if (cell.east) {
        neigbours.push_back(cell.east);
      }
      if (!neigbours.empty()) {
        int index = rand_num_gen::random_int(0, neigbours.size() - 1);
        Cell *neigbour = neigbours[index];
        cell.link(neigbour);
      }
    }
  }
}
