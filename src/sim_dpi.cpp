#include "Vtop__Dpi.h"

#include "./cpp/includes/Memory.hpp"
#include "./cpp/includes/Program.hpp"

Program program;

unsigned int get_instruction(unsigned int pc) {
  return program.get_instruction(pc);
}

Memory mem;

void mem_reset() { mem.reset(); }

unsigned int mem_load(unsigned int addr, const svBitVecVal *width) {
  return mem.load(addr, *width);
}

void mem_store(unsigned int addr, const svBitVecVal *width, int data) {
  mem.store(addr, *width, static_cast<uint32_t>(data));
}
