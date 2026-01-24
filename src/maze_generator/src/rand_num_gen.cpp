#include "maze_generator/rand_num_gen.hpp"
#include <random>
int rand_num_gen::random_int(int min, int max) {

  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(min, max);
  return dist(gen);
}
