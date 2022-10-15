#pragma once

#include "helpers.hpp"

#include <functional>
#include <limits>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

// TOKENS DEFINITION

class Tokens {
 public:
  static constexpr size_t npos = -1;
  static size_t ADD(const std::string &str, size_t index, const char **bad);
  static size_t SUB(const std::string &str, size_t index, const char **bad);
  static size_t MUL(const std::string &str, size_t index, const char **bad);
  static size_t DIV(const std::string &str, size_t index, const char **bad);
  static size_t Semicolon(const std::string &str, size_t index, const char **bad);
  static size_t Colon(const std::string &str, size_t index, const char **bad);
  static size_t LPAREN(const std::string &str, size_t index, const char **bad);
  static size_t RPAREN(const std::string &str, size_t index, const char **bad);
  static size_t QSMark(const std::string &str, size_t index, const char **bad);
  static size_t INTEGER(const std::string &str, size_t index, const char **bad);
  static size_t Eol(const std::string &str, size_t index, const char **bad);
  static size_t Eof(const std::string &str, size_t index, const char **bad);
};



// SCANNER DEFINITION

// You can freely add member fields and functions to this class, although it is not neccessary.
class Scanner {
 public:
  // constructor
  Scanner(std::string expression_list);
  Scanner();

  int lineNumber();
  int getIntegerValue();

  // In calculator.cpp, you really need to implement these two methods for the scanner to work.
  Token nextToken();
  void eatToken(Token);

 private:
  int line_;
  int value_;
  int index_;

  std::string expression_list_;
  const std::map<Token,
                 std::function<size_t(const std::string &, int, const char **)>>
      token_funcs_ = {{T_ADD, Tokens::ADD},
                      {T_SUB, Tokens::SUB},
                      {T_MUL, Tokens::MUL},
                      {T_DIV, Tokens::DIV},
                      {T_SEMICOLON, Tokens::Semicolon},
                      {T_COLON, Tokens::Colon},
                      {T_LPAREN, Tokens::LPAREN},
                      {T_RPAREN, Tokens::RPAREN},
                      {T_QSMARK, Tokens::QSMark},
                      {T_INTEGER, Tokens::INTEGER},
                      {T_EOL, Tokens::Eol},
                      {T_EOF, Tokens::Eof}};
};



// Parser DEFINITION

// You can freely add member fields and functions to this class.
class Parser {
 public:
  // constructor
  Parser(bool);

  bool evaluate;
  void parse();

 private:
  Scanner scanner;

  std::vector<int> Start();
  std::vector<int> ExpressionList();
  std::vector<int> ELHelper(std::vector<int> exp);
  size_t Expression();
  size_t ExpressionHelper(size_t e);
  size_t AdditiveExpression();
  size_t AEHelper(size_t ae);
  size_t Term();
  size_t THelper(size_t t);
  size_t Factor();

  static inline Token FilterTokens(Scanner &scanner) {
    while (scanner.nextToken() == T_EOL) {
      scanner.eatToken(T_EOL);
    }
    return scanner.nextToken();
  }
};
