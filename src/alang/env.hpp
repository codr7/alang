#ifndef ALANG_ENV_HPP
#define ALANG_ENV_HPP

#include <lgpp/env.hpp>
#include <lgpp/ops/add.hpp>
#include <lgpp/ops/and.hpp>
#include <lgpp/ops/branch.hpp>
#include <lgpp/ops/cp.hpp>
#include <lgpp/ops/drop.hpp>
#include <lgpp/ops/eq.hpp>
#include <lgpp/ops/go.hpp>
#include <lgpp/ops/gt.hpp>
#include <lgpp/ops/isa.hpp>
#include <lgpp/ops/lt.hpp>
#include <lgpp/ops/return.hpp>
#include <lgpp/ops/or.hpp>
#include <lgpp/ops/rec.hpp>
#include <lgpp/ops/recall.hpp>
#include <lgpp/ops/rot.hpp>
#include <lgpp/ops/sub.hpp>
#include <lgpp/ops/type_of.hpp>
#include <lgpp/ops/swap.hpp>
#include <lgpp/parser.hpp>
#include <lgpp/types.hpp>

#include "alang/vm.hpp"

namespace alang {
  using namespace std;
  using namespace lgpp;

  inline void let_label(Env&  env, Label& label) { let(env, *label.name, env.vm.Label, &label); }

  inline void let_macro(Env&  env, string id, Macro::Imp imp) { let(env, id, env.vm.Macro, id, imp); }

  inline void let_meta(Env&  env, Trait& type) { let(env, type.name, env.vm.Meta, &type); }

  inline void let_prim(Env&  env, string id, Prim::Imp imp) { let(env, id, env.vm.Prim, id, imp); }

  inline void init(lgpp::Env& env) {
    auto &vm = dynamic_cast<alang::VM&>(env.vm);
    
    let_meta(env, vm.Any);
    let_meta(env, vm.Coro);
    let_meta(env, vm.Int);
    let_meta(env, vm.Macro);
    let_meta(env, vm.Meta);
    let_meta(env, vm.Num);
    let_meta(env, vm.Pair);
    let_meta(env, vm.Prim);
    let_meta(env, vm.Seq);
    let_meta(env, vm.Stack);
    let_meta(env, vm.Sub);
    let_meta(env, vm.Thread);

    let_macro(env, "_", [](Toque& in, Thread &out, Env& env) {});

    let_macro(env, "=", [](Toque& in, Thread &out, Env& env) {
      compile(pop(in), in, out, env);
      compile(pop(in), in, out, env);
      emit<ops::Eq>(out);
    });

    let_macro(env, "<", [](Toque& in, Thread &out, Env& env) {
      compile(pop(in), in, out, env);
      compile(pop(in), in, out, env);
      emit<ops::Lt>(out);
    });
    
    let_macro(env, ">", [](Toque& in, Thread &out, Env& env) {
      compile(pop(in), in, out, env);
      compile(pop(in), in, out, env);
      emit<ops::Gt>(out);
    });

    let_macro(env, "+", [](Toque& in, Thread &out, Env& env) {
      compile(pop(in), in, out, env);
      compile(pop(in), in, out, env);
      emit<ops::Add>(out);
    });
    
    let_macro(env, "-", [](Toque& in, Thread &out, Env& env) {
      compile(pop(in), in, out, env);
      compile(pop(in), in, out, env);
      emit<ops::Sub>(out);
    });

    let_macro(env, "and", [](Toque& in, Thread &out, Env& env) {
      compile(pop(in), in, out, env);
      compile(pop(in), in, out, env);
      emit<ops::And>(out);
    });

    let_macro(env, "cp", [](Toque& in, Thread &out, Env& env) { emit<ops::Cp>(out); });
    let_macro(env, "d", [](Toque& in, Thread &out, Env& env) { emit<ops::Drop>(out); });

    let_macro(env, "if", [](Toque& in, Thread &out, Env& env) {
      Tok cond = pop(in), x = pop(in), y = pop(in);
      compile(cond, in, out, env);
      Label& xskip = push_label(out);
      emit<ops::Branch>(out, xskip);
      compile(x, in, out, env);
      Label& yskip = push_label(out);
      emit<ops::Go>(out, yskip);
      xskip.pc = emit_pc(out);
      compile(y, in, out, env);
      yskip.pc = emit_pc(out);
    });

    let_macro(env, "isa", [](Toque& in, Thread &out, Env& env) {
      compile(pop(in), in, out, env);
      compile(pop(in), in, out, env);
      emit<ops::Isa>(out);
    });

    let_macro(env, "let", [](Toque& in, Thread &out, Env& env) {
      string key = pop(in).as<toks::Id>().name;
      Label& skip = push_label(out);
      emit<ops::Go>(out, skip);
      PC start_pc = emit_pc(out);
      compile(pop(in), in, out, env);      
      emit<ops::Stop>(out);
      skip.pc = emit_pc(out);
      eval(out, start_pc);
      let(env, key, pop(get_stack(out)));
    });

    let_macro(env, "or", [](Toque& in, Thread &out, Env& env) {
      compile(pop(in), in, out, env);
      compile(pop(in), in, out, env);
      emit<ops::Or>(out);
    });

    let_macro(env, "rec", [](Toque& in, Thread &out, Env& env) { emit<ops::Rec>(out); });
    let_macro(env, "recall", [](Toque& in, Thread &out, Env& env) { emit<ops::Recall>(out); });
    let_macro(env, "rot", [](Toque& in, Thread &out, Env& env) { emit<ops::Rot>(out); });

    let_macro(env, "sub", [&vm](Toque& in, Thread &out, Env& env) {
      Label& skip = push_label(out);
      emit<ops::Go>(out, skip);
      Label& sub = push_label(out, nullopt, emit_pc(out));
      compile(pop(in), in, out, env);
      emit<ops::Return>(out);
      skip.pc = emit_pc(out);
      emit<ops::Push>(out, vm.Sub, &sub);
    });

    let_macro(env, "sw", [](Toque& in, Thread &out, Env& env) { emit<ops::Swap>(out); });

    let_macro(env, "type", [](Toque& in, Thread &out, Env& env) {
      compile(pop(in), in, out, env);
      emit<ops::TypeOf>(out);
    });

    let_prim(env, "say", [](Thread& thread, Pos pos) {
      Stack& s = get_stack(thread);
      say(pop(s), cout);
      cout << endl;
    });

    let(env, "T", vm.Bool, true);
    let(env, "F", vm.Bool, false);
  }
}

#endif
