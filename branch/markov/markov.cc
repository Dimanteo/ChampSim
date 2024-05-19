#include <map>

#include "msl/fwcounter.h"
#include "ooo_cpu.h"

template <std::size_t Power>
class markov_counter
{
  static constexpr std::size_t TABLE_SIZE = 1ul << Power;
  static constexpr std::size_t PREFIX_MASK = ~0ul + (1 << Power);
  std::array<long int, TABLE_SIZE> pattern_table{};
  std::size_t prefix = 0;

public:
  void update(uint8_t taken)
  {
    // freq 1 > freq 0 => freq 1 - freq 0 > 0
    // store only diff
    pattern_table[prefix] += taken ? 1 : -1;
    prefix = ((prefix << 1) | taken) & PREFIX_MASK;
  }

  uint8_t predict() const { return pattern_table[prefix] > 0; }
};

namespace
{
constexpr std::size_t MARKOV_TABLE_SIZE = 16384;
constexpr std::size_t MARKOV_PRIME = 16381;
constexpr std::size_t MARKOV_POWER = 2;

std::map<O3_CPU*, std::array<markov_counter<MARKOV_POWER>, MARKOV_TABLE_SIZE>> markov_table;
} // namespace

void O3_CPU::initialize_branch_predictor() {}

uint8_t O3_CPU::predict_branch(uint64_t ip)
{
  auto hash = ip % ::MARKOV_PRIME;
  auto value = ::markov_table[this][hash];

  return value.predict();
}

void O3_CPU::last_branch_result(uint64_t ip, uint64_t branch_target, uint8_t taken, uint8_t branch_type)
{
  auto hash = ip % ::MARKOV_PRIME;
  ::markov_table[this][hash].update(taken);
}
