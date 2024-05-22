#include <algorithm>
#include <cassert>
#include <map>
#include <vector>

#include "cache.h"

class pseudo_lru final
{
  // binary heap
  std::vector<uint32_t> heap;

public:
  pseudo_lru(size_t num_ways) : heap(num_ways, 0) {}

  uint32_t find_victim()
  {
    uint32_t way = 1;
    while (way < heap.size()) {
      way = heap[way] ? (way * 2 + 1) : (way * 2);
    }
    return way - heap.size();
  }

  void update(uint32_t way)
  {
    way += heap.size();
    while (way > 0) {
      bool is_right = way & 1;
      way = way >> 1;
      heap[way] = is_right ? 0 : 1;
    }
  }
};

namespace
{
std::map<CACHE*, std::vector<pseudo_lru>> last_used_cycles;
}

void CACHE::initialize_replacement()
{
  ::last_used_cycles[this] = std::vector<pseudo_lru>(NUM_SET, NUM_WAY);
}

uint32_t CACHE::find_victim(uint32_t triggering_cpu, uint64_t instr_id, uint32_t set, const BLOCK* current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
  auto&& lru = ::last_used_cycles[this][set];
  return lru.find_victim();
}

void CACHE::update_replacement_state(uint32_t triggering_cpu, uint32_t set, uint32_t way, uint64_t full_addr, uint64_t ip, uint64_t victim_addr, uint32_t type,
                                     uint8_t hit)
{
  // Mark the way as being used on the current cycle
  if (!hit || access_type{type} != access_type::WRITE) // Skip this for writeback hits
    ::last_used_cycles[this].at(set).update(way);
}

void CACHE::replacement_final_stats() {}
