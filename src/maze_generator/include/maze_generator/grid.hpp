#ifndef GRID_HPP
#define GRID_HPP

#include "maze_generator/cell.hpp"
#include <vector>
class Grid {
public:
  int rows, columns;
  std::vector<std::vector<Cell>> grid;
  Grid(int rows, int columns);
  int size();

private:
  std::vector<std::vector<Cell>> prepare_grid(int rows, int columns);
  void configure_cells();
  Cell *at(int row, int column);
  Cell *random_cell();
};

#endif // !GRID_HPP
