#include <vector>
#include <string>
#include <optional>
#include <fstream>

#include "driver.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "codegen.hpp"

/*
  void md_Parse();
  [[nodiscard]] int md_Run();

  [[nodiscard]] int md_Compile();
  [[nodiscard]] int md_Assembly();
  [[nodiscard]] int md_Link();
  std::optional<std::string> md_peek(size_t offset);
  std::string md_consume();
  size_t md_index = 1;
  std::string out("a.out");
  std::vector<std::string> md_args;
  std::vector<std::string> md_src;
  std::vector<std::string> md_flags;
  std::vector<std::string> md_linker_flags;
  std::vector<std::string> md_compile_flags;
*/


md_Driver::md_Driver(std::vector<std::string> args)
: md_args(args)
{}

std::optional<std::string> md_Driver::md_peek(size_t offset){
  if (md_index + offset >= md_args.size()){ 
    return {};
  }
  return md_args[md_index + offset];
}
std::string md_Driver::md_consume(){
  return md_args[md_index++];
}

void md_Driver::md_Parse(){
  while (md_peek().has_value()){
    std::string s = *md_peek();
    if (s[0] == '-'){
      switch (s[1]) {
        case 'l':
        case 'L':
          goto _links_flags;
        case 'f':
        case 'W':
          goto _Compile_flags;
        default:
          md_flags.emplace_back(s);
          md_consume();
          continue;
      }
        _links_flags:
          md_linker_flags.emplace_back();
          md_consume();
          continue;
        _Compile_flags:
          md_compile_flags.emplace_back(s);
          md_consume();
          continue;
    } else {
      md_src.emplace_back(s);
      md_consume();
      continue;
    } 
  }
}

