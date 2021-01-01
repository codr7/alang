#ifndef ALANG_VM_HPP
#define ALANG_VM_HPP

#include <lgpp/vm.hpp>
#include <lgpp/types/bool.hpp>
#include <lgpp/types/coro.hpp>
#include <lgpp/types/int.hpp>
#include <lgpp/types/label.hpp>
#include <lgpp/types/macro.hpp>
#include <lgpp/types/meta.hpp>
#include <lgpp/types/pair.hpp>
#include <lgpp/types/prim.hpp>
#include <lgpp/types/stack.hpp>

namespace alang {
  struct VM: lgpp::VM {
    VM();

    lgpp::Type<lgpp::Label *> Sub;
  };
}

#endif
