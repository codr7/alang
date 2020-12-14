#include "alang/parser.hpp"

int main() {
  using namespace alang;
  Parser p("repl");
  assert(parse(p, "foo + bar = 42") == 5);
  
  return 0;
}
