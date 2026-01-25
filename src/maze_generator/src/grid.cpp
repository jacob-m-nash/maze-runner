#include "maze_generator/grid.hpp"
#include "maze_generator/rand_num_gen.hpp"
#include <string>
#include <vector>
Grid::Grid(int rows, int columns) {
  this->rows_ = rows;
  this->columns_ = columns;
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
  if (row < 0 || row >= rows_ || col < 0 || col >= columns_) {
    return nullptr;
  }
  return &grid[row][col];
}
Cell *Grid::random_cell() {
  int row = RandomNumberGenerator::random_int(0, rows_ - 1);
  int col = RandomNumberGenerator::random_int(0, columns_ - 1);
  return at(row, col);
}
int Grid::size() { return rows_ * columns_; }

std::string Grid::to_string() {
  std::string output = "\n+";
  for (int i = 0; i < columns_; i++) {
    output += "---+";
  }
  output += "\n";
  for (auto &row : grid) {
    std::string top = "|";
    std::string bottom = "+";
    for (auto &cell : row) {
      std::string body = "   ";
      std::string east_boundary = cell.linked(cell.east) ? " " : "|";
      top += body + east_boundary;

      std::string south_boundary = cell.linked(cell.south) ? "   " : "---";
      std::string corner = "+";
      bottom += south_boundary + corner;
    }
    output += top + "\n";
    output += bottom + "\n";
  }
  return output;
}
