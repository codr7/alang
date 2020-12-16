#ifndef ALANG_TOKS_HPP
#define ALANG_TOKS_HPP

#include <lgpp/ops/push_stack.hpp>
#include <lgpp/ops/pop_stack.hpp>
#include <lgpp/toks.hpp>

namespace alang::toks {
  using namespace std;
  
  struct Stack: lgpp::toks::Group {
    template <typename...Args>
    Stack(Args&&...args): Group(forward<Args>(args)...) {}
  };
}

namespace lgpp::toks {
  
  template <>
  inline void compile(const Tok& tok, const alang::toks::Stack& imp, Parser& in, Thread& out, Env& env) {
    emit<ops::PushStack>(out);
    compile<Group>(tok, imp, in, out, env);
    emit<ops::PopStack>(out);
  }
    
  template <>
  inline void dump(const Tok &tok, const alang::toks::Stack &imp, ostream &out) {
    out << "(stack";
    auto i = 0;
      
    for (auto &t: imp.toks) {
      if (i++) { out << ' '; }
      dump(t, out);
    }
      
    out << ')';
  }
  
}


#endif
