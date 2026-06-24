#include <csignal>
#include <iostream>

#include "./dpi/Memory.hpp"
#include "./dpi/PixelDisplay.hpp"
#include "./dpi/Program.hpp"
#include "./top.hpp"

extern Memory mem;
extern PixelDisplay pd;
extern Program program;

bool stoped = false;
void handle_sigint(int sig) {
  stoped = true;
}

int main(int argc, char **argv) {
  signal(SIGINT, handle_sigint);

  Top top(argc, argv);

  std::string file = top._commandArgsPlusMatch("FILE=");
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

  // TODO: Make these sizes configurable via command line arguments.
  mem.init(1024 * 1024 * 1024); // 1 GiB

  if (!pd.init(128, 96, 6)) {
    pd.destroy();
    return 1;
  }

  while (!top.gotFinish()) {
    top.increase_time(1);

    if (top.time() > 2) {
      top.set_rst(0);
    }

    top.flip_clk();

    if (top.posedge_clk()) {
      stoped |= !pd.handle_event();
    }

    if (stoped) {
      std::cerr << "Simulation interrupted by user.\n";
      break;
    }

    top.eval();
  }

  pd.destroy();

  return 0;
}
