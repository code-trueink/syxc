#include <string>
#include <vector>
#include <optional>
#include <iostream>
#include "lexer.hpp"

Tokenizer::Tokenizer(std::string src)
: m_src(std::move(src)) {}

std::optional<char> Tokenizer::peek(size_t offset) const {
  if (m_index + offset >= m_src.length()) {
    return {};
  }
  return m_src[m_index + offset];
}

char Tokenizer::consume() {
  return m_src[m_index++];
}

void Tokenizer::modlogs() {
  logs << current_state.str() << '\n';
  current_state.str("");
  current_state.clear();
}

void Tokenizer::moderrors() {
  errors << current_state.str() << '\n';
  current_state.str("");
  current_state.clear();
}

std::vector<Token> Tokenizer::tokenize() {
  std::vector<Token> tokens;
  std::string buf;
  int line_count = 1;

  while (peek().has_value()) {

    if (std::isalpha((unsigned char)*peek())) {
      buf.push_back(consume());
      while (peek().has_value() &&
             std::isalnum((unsigned char)*peek())) {
        buf.push_back(consume());
      }

      if (buf == "exit") {
        current_state << "found key exit at line: " << line_count;
        tokens.push_back({TokenType::keywords, line_count, buf});
        modlogs();
      } else {
        current_state << "found varibles: " << buf
                      << " at line: " << line_count;
        tokens.push_back({TokenType::identifier, line_count, buf});
        modlogs();
      }
      buf.clear();
    }

    else if (std::isdigit((unsigned char)*peek())) {
      bool is_float = false;
      buf.push_back(consume());

      while (peek().has_value() &&
             std::isdigit((unsigned char)*peek())) {
        buf.push_back(consume());
      }

      if (peek().has_value() && *peek() == '.') {
        buf.push_back(consume());
        while (peek().has_value() &&
               std::isdigit((unsigned char)*peek())) {
          buf.push_back(consume());
        }
        is_float = true;
      }

      if (is_float) {
        current_state << "found float: " << buf
                      << " at line: " << line_count;
        tokens.push_back({TokenType::float_lit, line_count, buf});
        modlogs();
      } else {
        current_state << " found digit: " << buf
                      << " at line: " << line_count;
        tokens.push_back({TokenType::int_lit, line_count, buf});
        modlogs();
      }
      buf.clear();
    }

    else if (*peek() == ';') {
      consume();
      tokens.push_back({TokenType::semi_colon, line_count});
    }

    else if (*peek() == '(') {
      consume();
      tokens.push_back({TokenType::l_paren, line_count, "debug: ("});
    }

    else if (*peek() == ')') {
      consume();
      tokens.push_back({TokenType::r_paren, line_count, "debug: )"});
    }

    else if (*peek() == '{') {
      consume();
      tokens.push_back({TokenType::l_brace, line_count, "debug: {"});
    }

    else if (*peek() == '}') {
      consume();
      tokens.push_back({TokenType::r_brace, line_count, "debug: }"});
    }

    else if (*peek() == '[') {
      consume();
      tokens.push_back({TokenType::l_bracket, line_count, "debug: ["});
    }

    else if (*peek() == ']') {
      consume();
      tokens.push_back({TokenType::r_bracket, line_count, "debug: ]"});
    }

    else if (*peek() == '\n') {
      line_count++;
      consume();
    }

    else if (*peek() == '.') {
      tokens.push_back({TokenType::dot, line_count, "$"});
      consume();
    }

    else if (*peek() == '/' &&
             peek(1).has_value() &&
             *peek(1) == '/') {
      while (peek().has_value() &&
             !(consume() == '\n')) {}
      line_count++;
    }

    else if (*peek() == '/' &&
             peek(1).has_value() &&
             *peek(1) == '*') {
      consume();
      consume();

      while (peek().has_value()) {
        if (*peek() == '\n') {
          line_count++;
        }
        if (*peek() == '*' &&
            peek(1).has_value() &&
            *peek(1) == '/') {
          consume();
          consume();
          break;
        }
        consume();
      }
    }

    else if (std::isspace((unsigned char)*peek())) {
      consume();
    }

    else {
      current_state << "Syntax Failure: "
                    << consume()
                    << " unkown symbol";
      moderrors();
    }
  }

  m_index = 0;
  return tokens;
}

std::string Tokenizer::getlogs() {
  return Tokenizer::logs.str();
}

std::string Tokenizer::geterrors() {
  return Tokenizer::errors.str();
}

