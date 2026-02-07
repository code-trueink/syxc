#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <optional>

enum class TokenType {
  keywords,
  semi_colon,
  l_paren, r_paren,
  l_brace, r_brace,
  l_bracket, r_bracket,
  string_lit,
  int_lit,
  float_lit,
  char_lit,
  identifier,
  dot,
};

struct Token {
  TokenType type;
  int line;
  std::string value {};
};

class Tokenizer {
public:
  explicit Tokenizer(std::string src);

  std::vector<Token> tokenize();

  std::string getlogs();
  std::string geterrors();
private:

  const std::string& m_src;
  size_t m_index = 0;

  std::optional<char> peek(size_t offset = 0) const;
  char consume();

  std::stringstream current_state;

  std::stringstream logs;
  std::stringstream errors;


  void modlogs();
  void moderrors();

};


