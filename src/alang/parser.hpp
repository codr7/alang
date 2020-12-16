#ifndef ALANG_PARSER_HPP
#define ALANG_PARSER_HPP

#include <lgpp/parser.hpp>
#include "alang/toks.hpp"

namespace alang {
  using namespace std;

  inline void init_parser(lgpp::Parser &parser) {
    parser.alts.push_front(lgpp::parse_group('(', ')'));
    parser.alts.push_front(lgpp::parse_group<toks::Stack>('[', ']'));
  }
}

#endif
