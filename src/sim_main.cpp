#include <iostream>

#include "Vtop.h"

#include "./cpp/includes/ProgramLoader.hpp"

extern ProgramLoader program;

int main(int argc, char **argv) {
  const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};

  contextp->debug(0);
  contextp->threads(1);
  contextp->randReset(2);
  contextp->commandArgs(argc, argv);

#ifdef VM_TRACE
  contextp->traceEverOn(true);
#endif

  std::string file = contextp->commandArgsPlusMatch("FILE=");
  if (file.empty()) {
    std::cerr << "Error: No input file specified. Use +FILE=<path_to_file> to "
                 "specify the input file.\n";
    return 1;
  }
  try {
    // INFO: Call substr(6) to remove "FILE=" prefix.
    program.open(file.substr(6));
  } catch (std::runtime_error *e) {
    std::cerr << e->what() << "\n";
    return 1;
  }

  const std::unique_ptr<Vtop> top{new Vtop{contextp.get()}};

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
