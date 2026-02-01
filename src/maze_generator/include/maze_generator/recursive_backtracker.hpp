#ifndef RECURSIVE_BACKTRACKER_HPP
#define RECURSIVE_BACKTRACKER_HPP
#include "maze_generator/grid.hpp"
class RecursiveBacktracker {
public:
  static void on(Grid &grid) { on(grid.random_cell()); }
  static void on(Cell *start_at);
};

#endif // !RECURSIVE_BACKTRACKER_HPP
