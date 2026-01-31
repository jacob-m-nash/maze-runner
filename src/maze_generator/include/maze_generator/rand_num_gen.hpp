#ifndef RAND_NUM_GEN_HPP
#define RAND_NUM_GEN_HPP
#include <iterator>
class RandomNumberGenerator {
public:
  int static random_int(int min, int max);

  template <typename Container>
  static auto sample(const Container &container) // TODO: use
      -> decltype(*std::begin(container)) {
    auto size = std::distance(std::begin(container), std::end(container));
    auto index = randon_int(0, size - 1);
    auto it = std::begin(container);
    std::advance(it, index);
    return *it;
  }
};
#endif // !RAND_NUM_GEN_HPP
