#ifndef ALANG_ENV_HPP
#define ALANG_ENV_HPP

#include <lgpp/env.hpp>
#include <lgpp/ops/branch.hpp>
#include <lgpp/ops/cp.hpp>
#include <lgpp/ops/drop.hpp>
#include <lgpp/ops/go.hpp>
#include <lgpp/ops/isa.hpp>
#include <lgpp/ops/rot.hpp>
#include <lgpp/parser.hpp>
#include <lgpp/types.hpp>

namespace alang {
  using namespace std;
  using namespace lgpp;

  inline void set_label(Env&  env, Label& label) { set(env, label.name, types::Label, &label); }

  inline void set_macro(Env&  env, string id, Macro::Imp imp) { set(env, id, types::Macro, id, imp); }

  inline void set_meta(Env&  env, Trait& type) { set(env, type.name, types::Meta, &type); }

  inline void set_prim(Env&  env, string id, Prim::Imp imp) { set(env, id, types::Prim, id, imp); }

  inline void init(lgpp::Env& env) {
    set_meta(env, types::Any);
    set_meta(env, types::Coro);
    set_meta(env, types::Int);
    set_meta(env, types::Macro);
    set_meta(env, types::Meta);
    set_meta(env, types::Num);
    set_meta(env, types::Pair);
    set_meta(env, types::Prim);
    set_meta(env, types::Seq);
    set_meta(env, types::Stack);
    set_meta(env, types::Thread);

    set_macro(env, "_", [](Parser& in, Thread &out, Env& env) {});
    set_macro(env, "cp", [](Parser& in, Thread &out, Env& env) { emit<ops::Cp>(out); });
    set_macro(env, "d", [](Parser& in, Thread &out, Env& env) { emit<ops::Drop>(out); });

    set_macro(env, "if", [](Parser& in, Thread &out, Env& env) {
      Tok cond = pop(in), x = pop(in), y = pop(in);
      compile(cond, in, out, env);
      Label &xskip = push_label(out);
      emit<ops::Branch>(out, xskip);
      compile(x, in, out, env);
      Label &yskip = push_label(out);
      emit<ops::Go>(out, yskip);
      xskip.pc = emit_pc(out);
      compile(y, in, out, env);
      yskip.pc = emit_pc(out);
    });

    set_macro(env, ".isa", [](Parser& in, Thread &out, Env& env) {
      auto parent = pop(in);
      compile(parent, in, out, env);
      emit<ops::Isa>(out);
    });

    set_macro(env, "label", [](Parser& in, Thread &out, Env& env) {
      auto name = pop(in).as<toks::Id>().name;
      set_label(env, push_label(out, name, emit_pc(out)));
    });

    set_macro(env, "rot", [](Parser& in, Thread &out, Env& env) { emit<ops::Rot>(out); });

    set_prim(env, "=", [](Thread& thread, Pos pos) {
      auto& s = get_stack(thread);
      auto r = pop(s);
      push(s, types::Bool, pop(s) == r);
    });

    set_prim(env, "<", [](Thread& thread, Pos pos) {
      auto& s = get_stack(thread);
      auto r = pop(s);
      push(s, types::Bool, pop(s) < r);
    });

    set_prim(env, ">", [](Thread& thread, Pos pos) {
      auto& s = get_stack(thread);
      auto r = pop(s);
      push(s, types::Bool, pop(s) > r);
    });

    set(env, "T", types::Bool, true);
    set(env, "F", types::Bool, false);
  }
}

#endif
