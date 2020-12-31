#ifndef ALANG_PARSER_HPP
#define ALANG_PARSER_HPP

#include <lgpp/parser.hpp>
#include "alang/toks.hpp"

namespace alang {
  using namespace std;

  inline bool parse_id(Parser& parser, char c, istream& in) {
    auto p = parser.pos;
    if (!lgpp::parse_id_pred(parser, c, in, [](auto c) { return c != '('; })) { return false; }

    auto id = pop_back(parser).as<lgpp::toks::Id>().name;
    bool dot = id[0] == '.', call = false;
    if (dot) { id = id.substr(1); }
    
    if (in.get(c)) {
      if (c == '(') {
	if (parse_group('(', ')')(parser, c, in)) {
	  auto args = pop_back(parser);
	  push<toks::Call>(parser, p, id, args.as<lgpp::toks::Group>().toks);
	  call = true;
	}
      } else {
	in.unget();
      }
    }

    if (dot) {
      push<toks::DotId>(parser, p, id);
    } else if (!call) {
      push<lgpp::toks::Id>(parser, p, id);
    }
    
    return true;
  }

  inline void init(lgpp::Parser &parser) {
    parser.alts.push_back(parse_int);
    parser.alts.push_back(parse_group('(', ')'));
    parser.alts.push_back(parse_group<toks::Stack>('[', ']'));
    parser.alts.push_back(parse_id);
  }
}

#endif
