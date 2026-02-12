#include <string>
#include <cstring> 
#include <vector>
#include <optional>
#include <iostream>
#include "lexer.hpp"


namespace {
//operator table 
struct OpEntry {
  const char* lexeme;
  lc_TokenType type;
};

constexpr OpEntry op_table[] = {
  // compacted operators
  {"<<", lc_TokenType::bop_lshift},
  {">>", lc_TokenType::bop_rshift},
  {"+=", lc_TokenType::comp_emplus},
  {"-=", lc_TokenType::comp_eminus},
  {"*=", lc_TokenType::comp_emultiply},
  {"/=", lc_TokenType::comp_edivide},
  {"%=", lc_TokenType::comp_emodular},

  // compare
  {"==", lc_TokenType::cop_equal},
  {"!=", lc_TokenType::cop_diff},
  {"<=", lc_TokenType::cop_lequal},
  {">=", lc_TokenType::cop_mequal},

  // logical
  {"||", lc_TokenType::op_or},
  {"&&", lc_TokenType::op_and},

  // single-char operators
  {"=",  lc_TokenType::op_equal},
  {"+",  lc_TokenType::op_plus},
  {"-",  lc_TokenType::op_minus},
  {"/",  lc_TokenType::op_divide},
  {"%",  lc_TokenType::op_modular},
  {"!",  lc_TokenType::op_not},
  {"*",  lc_TokenType::sop_star},
  {"&",  lc_TokenType::sop_addressof},
  {"|",  lc_TokenType::bop_or},
  {"~",  lc_TokenType::bop_not},
  {"<",  lc_TokenType::cop_less},
  {">",  lc_TokenType::cop_more},
};
}

lc_Tokenizer::lc_Tokenizer(std::string src)
  : lc_src(std::move(src)) {}

std::optional<char> lc_Tokenizer::lc_peek(size_t offset) const {
  if (lc_index + offset >= lc_src.length()) {
    return {};
  }
  return lc_src[lc_index + offset];
}

char lc_Tokenizer::lc_consume() {
  return lc_src[lc_index++];
}

void lc_Tokenizer::lc_modlogs() {
  lc_logs << lc_current_state.str() << '\n';
  lc_current_state.str("");
  lc_current_state.clear();
}

void lc_Tokenizer::lc_moderrors() {
  lc_errors << lc_current_state.str() << '\n';
  lc_current_state.str("");
  lc_current_state.clear();
}

std::vector<lc_Token> lc_Tokenizer::lc_tokenize() {
  std::vector<lc_Token> tokens;
  std::string buf;
  int line_count = 1;

  while (lc_peek().has_value()) {

    if (std::isalpha((unsigned char)*lc_peek())) {
      buf.push_back(lc_consume());
      while (lc_peek().has_value() &&
             std::isalnum((unsigned char)*lc_peek())) {
        buf.push_back(lc_consume());
      }

      if (buf == "exit") {
        lc_current_state << "found key exit at line: " << line_count;
        tokens.push_back({lc_TokenType::keywords, line_count, buf});
        lc_modlogs();
      }
      else if (buf == "let"){
        lc_current_state << "found key let at line: " << line_count;
        tokens.push_back({lc_TokenType::keywords, line_count, buf});
        lc_modlogs();
      }
      else {
        lc_current_state << "found varibles: " << buf
                         << " at line: " << line_count;
        tokens.push_back({lc_TokenType::identifier, line_count, buf});
        lc_modlogs();
      }
      buf.clear();
    }
    
    else if (std::isdigit((unsigned char)*lc_peek())) {
      bool is_float = false;
      buf.push_back(lc_consume());

      while (lc_peek().has_value() &&
             std::isdigit((unsigned char)*lc_peek())) {
        buf.push_back(lc_consume());
      }

      if (lc_peek().has_value() && *lc_peek() == '.') {
        buf.push_back(lc_consume());
        if (lc_peek().has_value() && std::isdigit((unsigned char)*lc_peek())){
          while (lc_peek().has_value() &&
                 std::isdigit((unsigned char)*lc_peek())) {
            buf.push_back(lc_consume());
          }
        }
        else{
            lc_current_state << "Syntax Failure (float): "
              << "digit ends in dot without trailing number: "
              << buf
              << "at line: " << line_count;
            lc_moderrors();
            buf.clear();
            continue;
        }
        is_float = true;
      }

      if (is_float) {
        lc_current_state << "found float: " << buf
                         << " at line: " << line_count;
        tokens.push_back({lc_TokenType::float_lit, line_count, buf});
        lc_modlogs();
      } else {
        lc_current_state << " found digit: " << buf
                         << " at line: " << line_count;
        tokens.push_back({lc_TokenType::int_lit, line_count, buf});
        lc_modlogs();
      }
      buf.clear();
    }
    
    else {
      char c = *lc_peek();

      switch (c) {
        case ';': lc_consume(); tokens.push_back({lc_TokenType::semi_colon, line_count}); continue;
        case ':': lc_consume(); tokens.push_back({lc_TokenType::colon, line_count}); continue;
        case '(': lc_consume(); tokens.push_back({lc_TokenType::l_paren, line_count}); continue;
        case ')': lc_consume(); tokens.push_back({lc_TokenType::r_paren, line_count}); continue;
        case '{': lc_consume(); tokens.push_back({lc_TokenType::l_brace, line_count}); continue;
        case '}': lc_consume(); tokens.push_back({lc_TokenType::r_brace, line_count}); continue;
        case '[': lc_consume(); tokens.push_back({lc_TokenType::l_bracket, line_count}); continue;
        case ']': lc_consume(); tokens.push_back({lc_TokenType::r_bracket, line_count}); continue;
        case '.': lc_consume(); tokens.push_back({lc_TokenType::dot, line_count}); continue;
        case '\n': lc_consume(); ++line_count; continue;
      }

      if (c == '/' && lc_peek(1).has_value()) {
        if (*lc_peek(1) == '/') {
          while (lc_peek().has_value() && lc_consume() != '\n') {}
          ++line_count;
          continue;
        }
        if (*lc_peek(1) == '*') {
          lc_consume(); lc_consume();
          while (lc_peek().has_value()) {
            if (*lc_peek() == '\n') ++line_count;
            if (*lc_peek() == '*' && lc_peek(1).has_value() && *lc_peek(1) == '/') {
              lc_consume(); lc_consume();
              break;
            }
            lc_consume();
          }
          continue;
        }
      }

      bool matched = false;
      for (const auto& op : op_table) {
        size_t len = std::strlen(op.lexeme);
        bool ok = true;

        for (size_t i = 0; i < len; ++i) {
          if (!lc_peek(i).has_value() || *lc_peek(i) != op.lexeme[i]) {
            ok = false;
            break;
          }
        }

        if (ok) {
          for (size_t i = 0; i < len; ++i) lc_consume();
          tokens.push_back({op.type, line_count});
          matched = true;
          break;
        }
      }

      if (matched) continue;

      if (std::isspace((unsigned char)c)) {
        lc_consume();
        continue;
      }

      lc_current_state << "Syntax Failure: "
                       << lc_consume()
                       << " unknown symbol";
      lc_moderrors();
    }
  }

  lc_index = 0;
  return tokens;
}

std::string lc_Tokenizer::lc_getlogs() {
  return lc_logs.str();
}

std::string lc_Tokenizer::lc_geterrors() {
  return lc_errors.str();
}

