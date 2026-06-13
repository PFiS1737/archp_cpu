#include <fstream>
#include <iostream>
#include <stdexcept>

#include "./includes/ProgramLoader.hpp"

void ProgramLoader::open(const std::string &file_name) {
  std::ifstream ifs(file_name, std::ios::binary);
  if (!ifs) {
    throw new std::runtime_error("Failed to open file: " + file_name);
  }

  ifs.seekg(0, std::ios::end);
  size_t size = ifs.tellg();
  ifs.seekg(0, std::ios::beg);

  data.resize(size, 0);

  ifs.read(reinterpret_cast<char *>(data.data()), size);
}

uint32_t ProgramLoader::get_instruction(uint32_t pc) {
  uint32_t instr = 0;

  for (int i = 0; i < 4; ++i) {
    uint32_t byte = 0;
    if (pc + i < data.size()) {
      byte = data[pc + i];
    }
    instr |= (byte << (8 * i));
  }

  return instr;
}
