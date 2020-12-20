#ifndef ALANG_ENV_HPP
#define ALANG_ENV_HPP

#include <lgpp/env.hpp>
#include <lgpp/ops/go.hpp>
#include <lgpp/parser.hpp>
#include <lgpp/types.hpp>

namespace alang {
  using namespace std;
  using namespace lgpp;

  inline Env& set_label(Env&  env, string id, PC pc) { return set(env, id, types::Label, id, pc); }

  inline Env& set_macro(Env&  env, string id, Macro::Imp imp) { return set(env, id, types::Macro, id, imp); }

  inline void init(lgpp::Env& env) {
    set(env, "Coro", types::Meta, &types::Coro);
    set(env, "Int", types::Meta, &types::Int);
    set(env, "Nil", types::Meta, &types::Nil);
    set(env, "Macro", types::Meta, &types::Macro);
    set(env, "Meta", types::Meta, &types::Meta);
    set(env, "Num", types::Meta, &types::Num);
    set(env, "Pair", types::Meta, &types::Pair);
    set(env, "Seq", types::Meta, &types::Seq);
    set(env, "Stack", types::Meta, &types::Stack);
    set(env, "Thread", types::Meta, &types::Thread);

    set_macro(env, "go", [](Parser& in, Thread &out, Env& env) {
      auto idt = pop(in);
      auto id = idt.as<toks::Id>().name;
      auto target = find(env, id);
      if (!target) { throw EParse(idt.pos, "Unknown label: ", id); }
      emit<ops::Go>(out, target->as(types::Label));
    });
    
    set_macro(env, "label", [](Parser& in, Thread &out, Env& env) {
      auto name = pop(in).as<toks::Id>().name;
      cout << "label: " << name << endl;
      set_label(env, name, emit_pc(out));
    });
	
    set(env, "_", types::Nil, nullptr);
  }
}

#endif
