#include "alang/vm.hpp"

namespace alang {
  VM::VM(): Sub(*this, "Sub", {&Label}) {}
}
