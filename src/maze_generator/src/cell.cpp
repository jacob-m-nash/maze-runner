#include "maze_generator/cell.hpp"
#include <vector>

Cell::Cell(int row, int column) : row(row), column(column), links_{} {}

void Cell::link(Cell *cell, bool bidi) {
  links_[cell] = true;
  if (bidi) {
    cell->link(this, false);
  }
}

void Cell::unlink(Cell *cell, bool bidi) {
  links_.erase(cell);
  if (bidi) {
    cell->unlink(this, false);
  }
}
std::vector<Cell *> Cell::links() {
  std::vector<Cell *> keys;
  for (auto &pair : links_) {
    keys.push_back(pair.first);
  }
  return keys;
}
bool Cell::linked(Cell *cell) { return links_.find(cell) != links_.end(); }

std::vector<Cell *> Cell::neighbors() {
  std::vector<Cell *> neighbors;
  if (north)
    neighbors.push_back(north);
  if (south)
    neighbors.push_back(south);
  if (east)
    neighbors.push_back(east);
  if (west)
    neighbors.push_back(west);
  return neighbors;
}
