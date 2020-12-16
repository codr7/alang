#ifndef ALANG_ENV_HPP
#define ALANG_ENV_HPP

#include <lgpp/env.hpp>
#include <lgpp/types.hpp>

namespace alang {
  using namespace std;
  using namespace lgpp;

  inline void init(lgpp::Env& env) {
    set(env, "Coro", types::Meta, &types::Coro);
    set(env, "Int", types::Meta, &types::Int);
    set(env, "Nil", types::Meta, &types::Nil);
    set(env, "Num", types::Meta, &types::Num);
    set(env, "Pair", types::Meta, &types::Pair);
    set(env, "Seq", types::Meta, &types::Seq);
    set(env, "Stack", types::Meta, &types::Stack);
    set(env, "Thread", types::Meta, &types::Thread);

    set(env, "_", types::Nil, nullptr);
  }
}

#endif
