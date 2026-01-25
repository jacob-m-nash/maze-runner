#ifndef GRID_HPP
#define GRID_HPP

#include "maze_generator/cell.hpp"
#include <string>
#include <vector>
class Grid {
public:
  std::vector<std::vector<Cell>> grid;
  Grid(int rows, int columns);
  int size();
  std::string to_string();
  Cell *random_cell();

private:
  int rows_, columns_;
  std::vector<std::vector<Cell>> prepare_grid(int rows, int columns);
  void configure_cells();
  Cell *at(int row, int column);
};

#endif // !GRID_HPP
