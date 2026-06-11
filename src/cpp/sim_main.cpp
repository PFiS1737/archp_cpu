#include <iostream>

#include "archp_cpu.h"
#include "archp_cpu__Dpi.h"

#include "./includes/ProgramLoader.hpp"

ProgramLoader program;

unsigned int get_instruction(unsigned int pc) {
  return program.get_instruction(pc);
}

int main(int argc, char **argv) {
  const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};

  contextp->debug(0);
  contextp->threads(1);
  contextp->randReset(2);
  // contextp->traceEverOn(true);
  contextp->commandArgs(argc, argv);

  std::string file = contextp->commandArgsPlusMatch("FILE=");
  try {
    // INFO: Call substr(6) to remove "FILE=" prefix.
    program.open(file.substr(6));
  } catch (std::runtime_error *e) {
    std::cerr << e->what() << "\n";
    return 1;
  }

  const std::unique_ptr<archp_cpu> top{new archp_cpu{contextp.get()}};

  top->rst = 1;

  while (!contextp->gotFinish()) {
    contextp->timeInc(1);

    if (contextp->time() > 2) {
      top->rst = 0;
    }

    top->clk = !top->clk;

    top->eval();
  }

  top->final();

  return 0;
}
