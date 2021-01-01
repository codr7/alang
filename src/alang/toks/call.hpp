#ifndef ALANG_TOKS_CALL_HPP
#define ALANG_TOKS_CALL_HPP

#include <lgpp/tok.hpp>
#include <lgpp/ops/call.hpp>

namespace alang::toks {

  struct Call {
    template <typename...Args>
    Call(string target, Args&&...args): target(target), args(forward<Args>(args)...) {}
    
    string target;
    vector<Tok> args;
  };

}

namespace lgpp::toks {

  template <>
  inline void compile(const Tok& tok, const alang::toks::Call& imp, Toque& in, Thread& out, Env& env) {
    VM& vm = env.vm;
    auto found = find(env, imp.target);
    if (!found) { throw ECompile(tok.pos, "Unknown identifier: ", imp.target); }

    if (&get_type(*found) == &vm.Macro) {
      Toque as(imp.args.begin(), imp.args.end());
      found->as(vm.Macro).imp(as, out, env);
    } else {
      int i = 0, j = -1;
      bool skip = false;
      
      for (auto &a: imp.args) {
	if (auto id = a.try_as<toks::Id>(); id && id->name == "_") {
	  if (!skip) {
	    j = i;
	    skip = true;
	  }
	} else {
	  if (skip) {
	    if (j != -1) { emit<ops::Rot>(out, i - j, i - j); }
	    skip = false;
	  }
	  
	  compile(a, in, out, env);
	}
	
	i++;
      }

      emit<ops::Call>(out, *found);
    }
  }
    
  template <>
  inline void dump(const Tok &tok, const alang::toks::Call &imp, ostream &out) {
    out << "(call " << imp.target;
      
    for (auto &a: imp.args) {
      out << ' ';
      dump(a, out);
    }
      
    out << ')';
  }

}

#endif
