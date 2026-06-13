#include "Vtop__Dpi.h"

#include "./cpp/includes/ProgramLoader.hpp"

ProgramLoader program;

unsigned int get_instruction(unsigned int pc) {
  return program.get_instruction(pc);
}
