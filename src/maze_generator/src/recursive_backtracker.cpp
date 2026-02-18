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
    std::vector<Cell *> neighbours;
    for (Cell *cell : current->neighbours()) {
      if (cell->links().empty()) {
        neighbours.push_back(cell);
      }
    }
    if (neighbours.empty()) {
      stack.pop();
    } else {
      Cell *neighbour = RandomNumberGenerator::sample(neighbours);
      current->link(neighbour);
      stack.push(neighbour);
    }
  }
}
