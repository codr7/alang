#include <lgpp/env.hpp>
#include <lgpp/ops/stop.hpp>
#include <lgpp/repl.hpp>
#include <lgpp/stack.hpp>
#include <lgpp/vm.hpp>

#include "alang/env.hpp"
#include "alang/parser.hpp"
#include "alang/toks.hpp"

int main() {
  using namespace alang;
  using namespace lgpp;
  
  cout <<
    "Alang v0.2" << endl << endl <<
    "Press Return on empty line to eval." << endl <<
    "Empty eval clears stack and Ctrl+D exits." << endl << endl <<
    ": ";
  
  VM vm;

  Parser p("repl");
  init(p);

  Env env;
  init(env);
  
  stringstream buf;
  REPL repl(cin, cout);
  
  repl.on_getline = [&vm, &p, &env, &buf](auto &line) {
    if (line.empty()) {
      try {
	if (buf.tellp()) {
	  parse(p, buf.str());
	  stringstream().swap(buf);
	  auto &t = get_thread(vm);
	  auto start_pc = emit_pc(t);
	  compile(p, t, env);
	  
	  if (emit_pc(t) > start_pc) {
	    emit<ops::Stop>(t);
	    eval(t, start_pc);
	  }
	} else {
	  get_stack(vm).clear();
	}
      } catch (exception& e) { cout << e.what() << endl; }
      
      cout << get_stack(vm) << endl << ": ";
    } else {
      buf << line << endl;
      cout << " ";
    }
    
    return true;    
  };

  enter(repl);
  return 0;
}
