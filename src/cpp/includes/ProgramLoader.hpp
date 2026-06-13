#pragma once

#include <cstdint>
#include <string>
#include <vector>

class ProgramLoader {
public:
  void open(const std::string &file_name);
  uint32_t get_instruction(uint32_t pc);

private:
  std::vector<uint8_t> data;
};
