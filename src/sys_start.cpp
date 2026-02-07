#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "lexer.hpp"
#include "parser.hpp"
#include "codegen.hpp"

std::string tokens_to_asm(const std::vector<Token>& tokens) {
  std::stringstream output;
  output << "global _start\n_start:\n";

  for (size_t i = 0; i < tokens.size(); i++) {
    const Token& token = tokens.at(i);

    if (token.type == TokenType::keywords && token.value == "exit") {
      if (i + 1 < tokens.size() &&
          tokens.at(i + 1).type == TokenType::int_lit) {

        if (i + 2 < tokens.size() &&
            tokens.at(i + 2).type == TokenType::semi_colon) {

          output << "  mov rax, 60\n";
          output << "  mov rdi, " << tokens.at(i + 1).value << "\n";
          output << "  syscall";

        } else {
          std::cerr << "where my damn semi colon" << std::endl;
          exit(1);
        }
      }
    }
  }

  return output.str();
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Incorrect usage. Correct usage is:\n";
    std::cerr << "sysc <input.sys> <flags> <output>\n";
    return 1;
  }

  // Read input file
  std::string contents;
  {
    std::ifstream input(argv[1]);
    if (!input) {
      std::cerr << "Failed to open input file.\n";
      return 1;
    }

    std::stringstream ss;
    ss << input.rdbuf();
    contents = ss.str();
  }

  // Extract flags
  std::vector<std::string> flags;
  for (int i = 2; i < argc - 1; ++i) {
    flags.push_back(argv[i]);
  }

  std::string name = argv[argc - 1];
  std::string filename = "./" + name + ".asm";

  // Tokenize
  Tokenizer toker(contents);
  std::vector<Token> tokens = toker.tokenize();
  std::string logs = toker.getlogs();

  // Write assembly
  {
    std::ofstream file(filename);
    file << tokens_to_asm(tokens);
  }

  // Handle flags
  bool kout = false;
  for (const auto& f : flags) {
    if (f == "-l") {
      std::cout << logs << '\n';
    } else if (f == "-k" || f == "--kout") {
      kout = true;
    }
  }

  // Build commands
  std::stringstream commands;
  /*
  // Assemble
  commands << "nasm -felf64 " << filename;
  system(commands.str().c_str());
  commands.str("");
  commands.clear();

  // Link
  commands << "ld -o " << name << " " << name << ".o";
  system(commands.str().c_str());
  commands.str("");
  commands.clear();
  */

  // Optional cleanup
  if (!kout) {
    commands << "rm " << filename << " " << name << ".o";
    system(commands.str().c_str());
  }

  return 0;
}

