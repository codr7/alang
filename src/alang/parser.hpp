#ifndef ALANG_PARSER_HPP
#define ALANG_PARSER_HPP

#include <lgpp/parser.hpp>
#include "alang/toks.hpp"

namespace alang {
  using namespace std;

  inline bool parse_dot_id(Parser& parser, char c, istream& in) {
    Pos p = parser.pos;
    if (c != '.') { return false; }
    if (!lgpp::parse_tok(parser, in)) { throw EParse(parser.pos, "Missing dot target"); }

    auto idt = pop(parser);
    auto *id = idt.try_as<lgpp::toks::Id>();
    if (!id) { throw EParse(parser.pos, "Invalid dot target: ", idt); }
    push<toks::DotId>(parser, p, id->name);
    return true;
  }

  inline bool parse_id(Parser& parser, char c, istream& in) {
    auto p = parser.pos;
    if (!lgpp::parse_id_pred(parser, c, in, [](auto c) { return c != '('; })) { return false; }
    auto id = pop_back(parser).as<lgpp::toks::Id>().name;
    
    if (in.get(c)) {
      if (c == '(') {
	if (parse_group('(', ')')(parser, c, in)) {
	  auto args = pop_back(parser);
	  push<toks::Call>(parser, p, id, args.as<lgpp::toks::Group>().toks);
	  return true;
	}
      } else {
	in.unget();
      }
    }

    push<lgpp::toks::Id>(parser, p, id);
    
    return true;
  }

  inline void init(lgpp::Parser &parser) {
    parser.alts.push_back(parse_int);
    parser.alts.push_back(parse_group('(', ')'));
    parser.alts.push_back(parse_group<toks::Stack>('[', ']'));
    parser.alts.push_back(parse_dot_id);
    parser.alts.push_back(parse_id);
  }
}

#endif
