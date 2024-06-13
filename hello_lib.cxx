module;

import std;  // for cout and string

export module hello_lib;

export void hello_world(std::string const& name)
{
  std::cout << "Hello World! My name is " << name << std::endl;
}