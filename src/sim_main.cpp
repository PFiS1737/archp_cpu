#include <iostream>

#include "Vtop.h"

#include "./cpp/includes/Memory.hpp"
#include "./cpp/includes/Program.hpp"

extern Program program;
extern Memory mem;

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

  mem.init(1024 * 1024 * 1024); // 1 GiB

  const std::unique_ptr<Vtop> top{new Vtop{contextp.get()}};

  top->rst = 1;

  while (!contextp->gotFinish()) {
    contextp->timeInc(1);

    if (contextp->time() > 2) {
      top->rst = 0;
    }

    top->clk = !top->clk;

    if (top->clk) {
      if (top->top_input_enable) {
        uint32_t n;
        scanf("%d", &n);
        top->top_input_value = n;
      }

      if (top->top_output_enable) {
        printf("%d\n", top->top_output_value);
      }
    }

    top->eval();
  }

  top->final();

  return 0;
}
