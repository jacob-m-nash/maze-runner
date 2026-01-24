#include "maze_generator/grid.hpp"
#include "maze_generator/rand_num_gen.hpp"
#include <vector>
Grid::Grid(int rows, int columns) {
  this->rows = rows;
  this->columns = columns;
  this->grid = Grid::prepare_grid(rows, columns);
  configure_cells();
}
std::vector<std::vector<Cell>> Grid::prepare_grid(int rows, int columns) {
  std::vector<std::vector<Cell>> grid;
  for (int row = 0; row < rows; row++) {
    std::vector<Cell> row_cells;
    for (int column = 0; column < columns; column++) {
      row_cells.push_back(Cell(row, column));
    }
    grid.push_back(row_cells);
  }
  return grid;
}

void Grid::configure_cells() {
  for (auto &row : grid) {
    for (auto &cell : row) {
      int r = cell.row;
      int c = cell.column;
      cell.north = at(r - 1, c);
      cell.south = at(r + 1, c);
      cell.east = at(r, c + 1);
      cell.west = at(r, c - 1);
    }
  }
}
Cell *Grid::at(int row, int col) {
  if (row < 0 || row >= rows || col < 0 || col >= columns) {
    return nullptr;
  }
  return &grid[row][col];
}
Cell *Grid::random_cell() {
  int row = rand_num_gen::random_int(0, rows - 1);
  int col = rand_num_gen::random_int(0, columns - 1);
  return at(row, col);
}
int Grid::size() { return rows * columns; }
