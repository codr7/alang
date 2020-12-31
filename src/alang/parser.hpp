#ifndef ALANG_PARSER_HPP
#define ALANG_PARSER_HPP

#include <lgpp/parser.hpp>
#include "alang/toks.hpp"

namespace alang {
  using namespace std;

  inline size_t parse_id(Parser& parser, char c, istream& in) {
    if (!lgpp::parse_id_pred(parser, c, in, [](auto c) { return c != '('; })) { return 0; }

    if (in.get(c)) {
      if (c == '(') {
	if (parse_group('(', ')')(parser, c, in)) {
	  Tok args = pop_back(parser), id = pop_back(parser);
	  push<toks::Call>(parser, id.pos, id.as<lgpp::toks::Id>().name, args.as<lgpp::toks::Group>().toks);
	}
      } else {
	in.unget();
      }
    }

    return 1;
  }

  inline void init(lgpp::Parser &parser) {
    parser.alts.push_back(parse_int);
    parser.alts.push_back(parse_group('(', ')'));
    parser.alts.push_back(parse_group<toks::Stack>('[', ']'));
    parser.alts.push_back(parse_id);
  }
}

#endif
