#include <string>
#include <vector>
#include <optional>
#include <iostream>
#include "../../hpp_file/lexer.hpp"

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
      } else {
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
        while (lc_peek().has_value() &&
               std::isdigit((unsigned char)*lc_peek())) {
          buf.push_back(lc_consume());
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

    else if (*lc_peek() == ';') {
      lc_consume();
      tokens.push_back({lc_TokenType::semi_colon, line_count});
    }

    else if (*lc_peek() == '(') {
      lc_consume();
      tokens.push_back({lc_TokenType::l_paren, line_count, "debug: ("});
    }

    else if (*lc_peek() == ')') {
      lc_consume();
      tokens.push_back({lc_TokenType::r_paren, line_count, "debug: )"});
    }

    else if (*lc_peek() == '{') {
      lc_consume();
      tokens.push_back({lc_TokenType::l_brace, line_count, "debug: {"});
    }

    else if (*lc_peek() == '}') {
      lc_consume();
      tokens.push_back({lc_TokenType::r_brace, line_count, "debug: }"});
    }

    else if (*lc_peek() == '[') {
      lc_consume();
      tokens.push_back({lc_TokenType::l_bracket, line_count, "debug: ["});
    }

    else if (*lc_peek() == ']') {
      lc_consume();
      tokens.push_back({lc_TokenType::r_bracket, line_count, "debug: ]"});
    }

    else if (*lc_peek() == '\n') {
      line_count++;
      lc_consume();
    }

    else if (*lc_peek() == '.') {
      tokens.push_back({lc_TokenType::dot, line_count, "$"});
      lc_consume();
    }

    else if (*lc_peek() == '/' &&
             lc_peek(1).has_value() &&
             *lc_peek(1) == '/') {
      while (lc_peek().has_value() &&
             !(lc_consume() == '\n')) {}
      line_count++;
    }

    else if (*lc_peek() == '/' &&
             lc_peek(1).has_value() &&
             *lc_peek(1) == '*') {
      lc_consume();
      lc_consume();

      while (lc_peek().has_value()) {
        if (*lc_peek() == '\n') {
          line_count++;
        }
        if (*lc_peek() == '*' &&
            lc_peek(1).has_value() &&
            *lc_peek(1) == '/') {
          lc_consume();
          lc_consume();
          break;
        }
        lc_consume();
      }
    }

    else if (std::isspace((unsigned char)*lc_peek())) {
      lc_consume();
    }

    else {
      lc_current_state << "Syntax Failure: "
                       << lc_consume()
                       << " unkown symbol";
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

