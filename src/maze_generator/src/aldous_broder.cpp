#include "maze_generator/aldous_broder.hpp"
#include "maze_generator/rand_num_gen.hpp"
void AldousBroder::on(Grid &grid) {
  Cell *cell = grid.random_cell();
  int unvisitied = grid.size() - 1;
  while (unvisitied > 0) {
    int index =
        RandomNumberGenerator::random_int(0, cell->neighbors().size() - 1);
    Cell *neighbor = cell->neighbors()[index];
    if (neighbor->links().size() == 0) {
      cell->link(neighbor);
      unvisitied -= 1;
    }
    cell = neighbor;
  }
}
