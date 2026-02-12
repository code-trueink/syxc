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
  op_equal,              // those operator that
  op_minus, op_plus,      // start with op_* are
  op_divide, op_modular, // normal operator
  op_not, op_or, op_and, // ! ; || ; &&
  //special operator
  sop_star,      //either multiply or pointer: *
  sop_addressof, //either 'bitwise and' or addressof: &
  //bitwise op 
  bop_not, bop_or, bop_rshift, bop_lshift, // ~ ; | ; >> ; <<
  cop_equal, cop_lequal, cop_mequal, // compare operator: '==' ; '<=' ; '>='
  cop_diff, cop_less, cop_more,      // compare operator: '!=' ; '<'  ; '>'
  //compacted operator
  comp_eminus, comp_emplus,
  comp_edivide, comp_emodular,
  comp_emultiply,
  string_lit,
  int_lit,
  float_lit,
  char_lit,
  identifier,
  dot,
  colon,
  ERROR,
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


