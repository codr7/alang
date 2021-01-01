#ifndef ALANG_TOKS_HPP
#define ALANG_TOKS_HPP

#include <lgpp/ops/call.hpp>
#include <lgpp/ops/push_stack.hpp>
#include <lgpp/ops/pop_stack.hpp>
#include <lgpp/ops/rot.hpp>
#include <lgpp/toks.hpp>

namespace alang::toks {
  using namespace std;

  struct Call {
    template <typename...Args>
    Call(string target, Args&&...args): target(target), args(forward<Args>(args)...) {}
    
    string target;
    vector<Tok> args;
  };

  struct CTE {
    CTE(Tok expr): expr(expr) {}
    Tok expr;
  };

  struct DotId: lgpp::toks::Id {
    DotId(string name): Id(name) {}
  };
    
  struct Stack: lgpp::toks::Group {
    template <typename...Args>
    Stack(Args&&...args): Group(forward<Args>(args)...) {}
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
	    if (j != -1) {
	      emit<ops::Rot>(out, i - j, i - j);
	    }
	    
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
  
  template <>
  inline void compile(const Tok& tok, const alang::toks::DotId& imp, Toque& in, Thread& out, Env& env) {    
    in.emplace_back(tok.pos, Id("_"));
    lgpp::toks::compile<Id>(tok, imp, in, out, env);
  }

  template <>
  inline void dump(const Tok &tok, const alang::toks::DotId &imp, ostream &out) {
    out << '.';
    lgpp::toks::dump<Id>(tok, imp, out);
  }

  template <>
  inline void compile(const Tok& tok, const alang::toks::Stack& imp, Toque& in, Thread& out, Env& env) {
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
