#ifndef ALANG_VM_HPP
#define ALANG_VM_HPP

#include <lgpp/vm.hpp>

namespace alang {
  
  struct VM: lgpp::VM {
    VM();

    lgpp::Type<lgpp::Label *> Sub;
  };
  
}

#endif
