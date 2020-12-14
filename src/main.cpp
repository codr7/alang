#include <lgpp/env.hpp>
#include <lgpp/ops/stop.hpp>
#include <lgpp/repl.hpp>
#include <lgpp/stack.hpp>
#include <lgpp/vm.hpp>
#include "alang/parser.hpp"

int main() {
  using namespace alang;

  cout <<
    "Alang v0.1" << endl << endl <<
    "Press Return on empty line to eval." << endl <<
    "Empty eval clears stack and Ctrl+D exits." << endl << endl <<
    ": ";

  lgpp::VM vm;
  Parser p("repl");
  p.alts.push_front(lgpp::parse_group('(', ')'));

  lgpp::Stack s;
  lgpp::Env env;
  stringstream buf;
  lgpp::REPL repl(cin, cout);
  
  repl.on_getline = [&vm, &p, &s, &env, &buf](auto &line) {
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
	    lgpp::eval(t, start_pc, s);
	  }
	} else {
	  s.clear();
	}
      } catch (exception& e) { cout << e.what() << endl; }
      
      cout << s << endl << ": ";
    } else {
      buf << line;
      cout << ". ";
    }
    
    return true;    
  };

  lgpp::enter(repl);
  return 0;
}
