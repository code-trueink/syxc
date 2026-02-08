#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <optional>

enum class lc_TokenType {
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

struct lc_Token {
  lc_TokenType type;
  int line;
  std::string value {};
};

class lc_Tokenizer {
public:
  explicit lc_Tokenizer(std::string src);

  std::vector<lc_Token> lc_tokenize();

  std::string lc_getlogs();
  std::string lc_geterrors();
private:

  const std::string& lc_src;
  size_t lc_index = 0;

  std::optional<char> lc_peek(size_t offset = 0) const;
  char lc_consume();

  std::stringstream lc_current_state;

  std::stringstream lc_logs;
  std::stringstream lc_errors;


  void lc_modlogs();
  void lc_moderrors();

};


