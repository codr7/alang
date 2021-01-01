#include <iostream>

#include <lgpp/env.hpp>
#include <lgpp/ops/stop.hpp>
#include <lgpp/repl.hpp>
#include <lgpp/stack.hpp>

#include "alang/env.hpp"
#include "alang/parser.hpp"
#include "alang/toks.hpp"
#include "alang/vm.hpp"

int main() {
  using namespace alang;
  
  cout <<
    "Alang v0.3" << endl << endl <<
    "Press Return on empty line to eval." << endl <<
    "Empty eval clears stack and Ctrl+D exits." << endl << endl <<
    ": ";
  
  alang::VM vm;
  bool debug = false;
  
  Parser p(vm, "repl");
  init(p);

  Env env(vm);
  init(env);
  
  stringstream buf;
  REPL repl(cin, cout);
  
  repl.on_getline = [&vm, &debug, &p, &env, &buf](auto &line) {
    if (line.empty()) {
      try {
	if (buf.tellp()) {
	  string in = buf.str();
	  stringstream().swap(buf);
	  parse(p, in);
	  Thread& t = get_thread(vm);
	  PC start_pc = emit_pc(t);
	  compile(p, t, env);
	  
	  if (emit_pc(t) > start_pc) {
	    emit<ops::Stop>(t);
	    eval(t, start_pc);
	  }
	} else {
	  get_stack(vm).clear();
	}
      } catch (exception& e) {	
	if (debug) { throw; }
	cout << "Error: " << e.what() << endl;
      }
      
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
