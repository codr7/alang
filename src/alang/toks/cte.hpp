#ifndef ALANG_TOKS_CTE_HPP
#define ALANG_TOKS_CTE_HPP

#include <lgpp/tok.hpp>

namespace alang::toks {

  struct CTE {
    CTE(Tok expr): expr(expr) {}
    Tok expr;
  };
  
}

namespace lgpp::toks {

  template <>
  inline void compile(const Tok& tok, const alang::toks::CTE& imp, Toque& in, Thread& out, Env& env) {
    Label& skip = push_label(out);
    emit<ops::Go>(out, skip);
    PC start_pc = emit_pc(out);
    Toque ein;
    compile(imp.expr, ein, out, env);      
    emit<ops::Stop>(out);
    skip.pc = emit_pc(out);
    eval(out, start_pc);
  }

  template <>
  inline void dump(const Tok &tok, const alang::toks::CTE &imp, ostream &out) {
    out << '@';
    dump(imp.expr, out);
  }

}

#endif
