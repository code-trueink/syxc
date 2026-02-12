#pragma once

#include <vector>
#include <string>
#include <optional>

class md_Driver{
public:
  explicit md_Driver(std::vector<std::string> args);
  void md_Parse();
  [[nodiscard]] int md_Run();
private:
  [[nodiscard]] int md_Compile();
  [[nodiscard]] int md_Assembly();
  [[nodiscard]] int md_Link();
  std::optional<std::string> md_peek(size_t offset=0);
  std::string md_consume();
  size_t md_index = 1;
  std::string out = "a.out";
  std::vector<std::string> md_args;
  std::vector<std::string> md_src;
  std::vector<std::string> md_flags;
  std::vector<std::string> md_linker_flags;
  std::vector<std::string> md_compile_flags;
};

