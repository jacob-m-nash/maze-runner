#include "maze_generator/aldous_broder.hpp"
#include "maze_generator/rand_num_gen.hpp"
void AldousBroder::on(Grid &grid) {
  Cell *cell = grid.random_cell();
  int unvisitied = grid.size() - 1;
  while (unvisitied > 0) {
    int index =
        RandomNumberGenerator::random_int(0, cell->neighbours().size() - 1);
    Cell *neighbours = cell->neighbours()[index];
    if (neighbours->links().size() == 0) {
      cell->link(neighbours);
      unvisitied -= 1;
    }
    cell = neighbours;
  }
}
