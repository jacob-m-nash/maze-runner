#include "maze_generator/side_winder.hpp"
#include "maze_generator/rand_num_gen.hpp"
#include <vector>
void SideWinder::on(Grid &grid) {
  for (auto &row : grid.grid) {
    std::vector<Cell *> run;
    for (auto &cell : row) {
      run.push_back(&cell);
      bool at_eastern_boundry = cell.east == nullptr;
      bool at_northen_boundry = cell.north == nullptr;

      if (at_eastern_boundry ||
          (!at_northen_boundry &&
           RandomNumberGenerator::random_int(0, 1) == 0)) {
        int index = RandomNumberGenerator::random_int(0, run.size() - 1);
        Cell *member = run[index];
        if (member->north) {
          member->link(member->north);
        }
        run.clear();
      } else {
        cell.link(cell.east);
      }
    }
  }
}
