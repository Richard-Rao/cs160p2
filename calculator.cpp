#include "calculator.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <climits>

// Tokens implementation

// You need to fill these methods with the appropriate code for it to work as described in the project description.
// Exmaples of ADD and Eof is given.
size_t Tokens::ADD(const std::string &str, size_t index, const char **bad) { 
  *bad = &str[index];
  if (index + 1 > str.size()) return -1;
  return str[index] == '+' ? 1 : -1;
}

size_t Tokens::SUB(const std::string &str, size_t index, const char **bad) { 
  *bad = &str[index];
  if (index + 1 > str.size()) return -1;
  return str[index] == '-' ? 1 : -1;
}

size_t Tokens::MUL(const std::string &str, size_t index, const char **bad) { 
  *bad = &str[index];
  if (index + 1 > str.size()) return -1;
  return str[index] == '*' ? 1 : -1;
}

size_t Tokens::DIV(const std::string &str, size_t index, const char **bad) { 
  *bad = &str[index];
  if (index + 1 > str.size()) return -1;
  return str[index] == '/' ? 1 : -1;
}

size_t Tokens::Semicolon(const std::string &str, size_t index, const char **bad) { 
  *bad = &str[index];
  if (index + 1 > str.size()) return -1;
  return str[index] == ';' ? 1 : -1;
}

size_t Tokens::Colon(const std::string &str, size_t index, const char **bad) { 
  *bad = &str[index];
  if (index + 1 > str.size()) return -1;
  return str[index] == ':' ? 1 : -1;
}

size_t Tokens::LPAREN(const std::string &str, size_t index, const char **bad) { 
  *bad = &str[index];
  if (index + 1 > str.size()) return -1;
  return str[index] == '(' ? 1 : -1;
}

size_t Tokens::RPAREN(const std::string &str, size_t index, const char **bad) { 
  *bad = &str[index];
  if (index + 1 > str.size()) return -1;
  return str[index] == ')' ? 1 : -1;
}

size_t Tokens::QSMark(const std::string &str, size_t index, const char **bad) { 
  *bad = &str[index];
  if (index + 1 > str.size()) return -1;
  return str[index] == '?' ? 1 : -1;
}

size_t Tokens::INTEGER(const std::string &str, size_t index, const char **bad) { 
  *bad = &str[index];
  if (index + 1 > str.size() || !isdigit(str[index])) return -1;
  int size = 0;
  while(isdigit(str[index])){
    index++;
    size++;
  }
  
  return size;
}

size_t Tokens::Eol(const std::string &str, size_t index, const char **bad) { 
  *bad = &str[index];
  if (index + 1 > str.size()) return -1;
  return str[index] == '\n' ? 1 : -1;
}

size_t Tokens::Eof(const std::string &str, size_t index, const char **bad) {
  *bad = &str[index];
  *bad = nullptr;
  return str.size() == index ? 0 : -1;
}



// Scanner implementation

// You can modify this constructor, although it is not neccessary.
Scanner::Scanner() : Scanner("") {
  std::stringstream expressions;
  std::string line;
  while (std::getline(std::cin, line)) {
    expressions << line << '\n';
  }
  expression_list_ = expressions.str();
  expression_list_.erase(expression_list_.size() - 1);
};

Scanner::Scanner(std::string expression_list)
    : line_(1),
      value_(0),
      index_(0),
      expression_list_(std::move(expression_list)) {}


// You need to fill these methods with the appropriate code for it to work as described in the project description.
Token Scanner::nextToken() {
  while (expression_list_[index_] == ' ' || expression_list_[index_] == '\t')
    ++index_;
  const char *bad = nullptr;
  for (auto it = token_funcs_.begin(); it != token_funcs_.end(); ++it) {
    const char *curr_bad = nullptr;
    size_t length = it->second(expression_list_, index_, &curr_bad);  
    if (curr_bad > bad) bad = curr_bad;
    // I am a placeholder. Change me into something that can actually decypher the next token without consuming it. 
    // Also, if I encounter an invalid input, I can throw the Error using methods defined in helpers.hpp
    //
    if (length != -1){
      if (it->first == T_INTEGER){
        value_ = stoi(expression_list_.substr(index_, length));
      } 
      return it->first;
    }
  }
  scanError(line_, *bad);
}

void Scanner::eatToken(Token toConsume) {
  Token t = nextToken();

  if (toConsume == t){
    size_t size = 1;
    
    if (t == T_INTEGER){
      size_t temp = 0;
      while(isdigit(expression_list_[index_ + temp])){
        temp++;
      }
      size = temp;
    }

    if(t == T_EOL){
      line_ += 1;
    }
    index_ = index_ + size;

  }else{
    mismatchError(line_, toConsume, t);
  }
}



int Scanner::lineNumber() { return line_; }

int Scanner::getIntegerValue() { return value_; }


// Parser implementation

// You can modify this constructor, although it is not neccessary.
Parser::Parser(bool eval) : evaluate(eval) {}

void Parser::parse() {
  std::vector<int> expressions = Start();
  if (evaluate) {
    // output the evaluate result
    for (int i = 0; i < expressions.size(); ++i) {
      std::cout << expressions[i];
      if (i + 1 < expressions.size()) {
        std::cout << std::endl;
      }
    }
  }
}



// You need to fill these methods with the appropriate code for it to work as described in the project description. 
// Implement a recursive descent parser starting from me. Add other methods for different recursive steps.
// Depending on how you designed your grammar, how many levels of operator precedence you need, etc., you might end up with more or less steps in the process.
// The example of Start() is given
std::vector<int> Parser::Start() {
  std::vector<int> result;
  switch (FilterTokens(scanner)) {
    case T_INTEGER:
      result = ExpressionList();
      break;
      
    case T_LPAREN:
      // Start -> ExpressionList
      result = ExpressionList();
      break;
    
    case T_EOF:
      return result;
      
    default:

      parseError(scanner.lineNumber(), scanner.nextToken());
      break;
  }
  return result;
}

std::vector<int> Parser::ExpressionList() {
  std::vector<int> result;
  size_t expr = Expression();
  result.push_back(expr);
  result = ELHelper(result);


  return result;

}

std::vector<int> Parser::ELHelper(std::vector<int> expList) {
  std::vector<int> result = expList;
  switch (FilterTokens(scanner)){
    case T_SEMICOLON:
    {
      scanner.eatToken(T_SEMICOLON);
      size_t expr = Expression();
      result.push_back(expr);      
      break;
    }

    default:
      return expList;
      break;

    return ELHelper(result);
  }
}

size_t Parser::Expression() {
  size_t result = 0;
  size_t exp = AdditiveExpression();
  result = ExpressionHelper(exp);

  return result;
  
}
size_t Parser::ExpressionHelper(size_t e){
  size_t result = 0;
  switch(FilterTokens(scanner)){
    case T_QSMARK:
    {
      scanner.eatToken(T_QSMARK);
      size_t exp1 = Expression();
      scanner.eatToken(T_COLON);
      size_t exp2 = Expression();

      if (e){
        result = exp1;
      }else{
        result = exp2;
      }
      break;
    }
    
    default:
      return e;
      break;
  }
  return ExpressionHelper(result);
}

size_t Parser::AdditiveExpression() {
  size_t result = 0;
  size_t t = Term();
  result = AEHelper(t);
  return result;
}

size_t Parser::AEHelper(size_t t) {
  size_t result = 0;
  size_t nextTerm = 0;
  switch (FilterTokens(scanner)){
    case T_ADD:
      scanner.eatToken(T_ADD);
      nextTerm = Term();
      result += nextTerm;
      break;
      
    case T_SUB:
      scanner.eatToken(T_SUB);
      nextTerm = Term();
      result -= nextTerm;
      break;

    default:
      return t;
      break;
  }
  int res = static_cast<long>(result);
  if (std::abs(res) > INT_MAX){
    outOfBoundsError(scanner.lineNumber(), result);
  } 
  return AEHelper(result);
}


size_t Parser::Term() {
  size_t result = 0;
  size_t f = Factor();
  result = THelper(f);
  return result;
}

size_t Parser::THelper(size_t f) {
  size_t result = f;
  size_t nextTerm;
  switch (FilterTokens(scanner)){
    case T_MUL:
    {
      scanner.eatToken(T_MUL);
      nextTerm = Factor();
      result *= nextTerm;
      break;
    }

    case T_DIV:
    {
      scanner.eatToken(T_DIV);
      nextTerm = Factor();
      if (nextTerm == 0){
        divideByZeroError(scanner.lineNumber(), f);
      }
      result /= nextTerm;
      break;
    }

    default:
      return f;
  }
  return THelper(result);
}

size_t Parser::Factor() {
  size_t result = 0;
  switch (FilterTokens(scanner)){
    case T_LPAREN:
    {
    
      scanner.eatToken(T_LPAREN);
      size_t exp = Expression();
      scanner.eatToken(T_RPAREN);
      result = exp;
      break;
    }
    
    case T_INTEGER:
    {
      scanner.eatToken(T_INTEGER);
      result = scanner.getIntegerValue();
      break;
    }

    default:
      parseError(scanner.lineNumber(), scanner.nextToken());
      break;
  }
  return result;
}
