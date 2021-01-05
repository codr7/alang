#ifndef ALANG_TOKS_DOT_ID_HPP
#define ALANG_TOKS_DOT_ID_HPP

#include <lgpp/tok.hpp>

namespace alang::toks {

  struct DotId: lgpp::toks::Id {
    DotId(string name): Id(name) {}
  };
  
}

namespace lgpp::toks {

  template <>
  inline void compile(const Tok& tok, const alang::toks::DotId& imp, Toque& in, Thread& out, Env& env) {    
    in.emplace_front(tok.pos, Id("_"));
    lgpp::toks::compile<Id>(tok, imp, in, out, env);
  }

  template <>
  inline void dump(const Tok &tok, const alang::toks::DotId &imp, ostream &out) {
    out << '.';
    lgpp::toks::dump<Id>(tok, imp, out);
  }

}

#endif
