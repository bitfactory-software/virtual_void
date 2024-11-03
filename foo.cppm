// Global module fragment where #includes can happen
module;
#include <iostream>

// first thing after the Global module fragment must be a module command
export module foo;

export class foo {
public:
  foo()= default;
  ~foo()= default;
  void helloworld(){ std::cout << "hello world\n"; }
};

//foo::foo() = default;
//foo::~foo() = default;
//void foo::helloworld() { std::cout << "hello world\n"; }