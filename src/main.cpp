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

  cout <<
    "Alang v0.1" << endl << endl <<
    "Press Return on empty line to eval." << endl <<
    "Empty eval clears stack and Ctrl+D exits." << endl << endl <<
    ": ";
  
  lgpp::VM vm;

  lgpp::Parser p("repl");
  init(p);

  lgpp::Env env;
  init(env);
  
  stringstream buf;
  lgpp::REPL repl(cin, cout);
  
  repl.on_getline = [&vm, &p, &env, &buf](auto &line) {
    if (line.empty()) {
      try {
	if (buf.tellp()) {
	  lgpp::parse(p, buf.str());
	  stringstream().swap(buf);
	  auto &t = lgpp::get_thread(vm);
	  auto start_pc = lgpp::emit_pc(t);
	  lgpp::compile(p, t, env);
	  
	  if (lgpp::emit_pc(t) > start_pc) {
	    lgpp::emit<lgpp::ops::Stop>(t);
	    lgpp::eval(t, start_pc);
	  }
	} else {
	  lgpp::get_stack(vm).clear();
	}
      } catch (exception& e) { cout << e.what() << endl; }
      
      cout << lgpp::get_stack(vm) << endl << ": ";
    } else {
      buf << line << endl;
      cout << " ";
    }
    
    return true;    
  };

  lgpp::enter(repl);
  return 0;
}
