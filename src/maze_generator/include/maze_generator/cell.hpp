#ifndef CELL_HPP
#define CELL_HPP

#include <unordered_map>
#include <vector>
class Cell {
public:
  Cell(int row, int column);
  int row, column;
  Cell *north, *south, *east, *west;
  std::unordered_map<Cell *, bool> links_;
  void link(Cell *cell, bool bidi = true);
  void unlink(Cell *cell, bool bidi = true);
  std::vector<Cell *> links();
  bool linked(Cell *cell);
  std::vector<Cell *> neighbors();
};
#endif // !CELL_HPP
