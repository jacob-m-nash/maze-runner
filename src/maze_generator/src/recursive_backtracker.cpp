#include "maze_generator/recursive_backtracker.hpp"
#include "maze_generator/cell.hpp"
#include "maze_generator/rand_num_gen.hpp"
#include <stack>
#include <vector>
void RecursiveBacktracker::on(Cell *start_on) {
  std::stack<Cell *> stack;
  stack.push(start_on);
  while (!stack.empty()) {
    Cell *current = stack.top();
    std::vector<Cell *> neighbors;
    for (Cell *cell : current->neighbors()) {
      if (cell->links().empty()) {
        neighbors.push_back(cell);
      }
    }
    if (neighbors.empty()) {
      stack.pop();
    } else {
      Cell *neighbor = RandomNumberGenerator::sample(neighbors);
      current->link(neighbor);
      stack.push(neighbor);
    }
  }
}
