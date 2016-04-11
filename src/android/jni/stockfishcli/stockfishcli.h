#include <iostream>
#include <sstream>
#include <string>
#include <position.h>

namespace stockfishcli
{
  void position(Position& pos, std::istringstream& is);

  void setoption(std::istringstream& is);

  void go(const Position& pos, std::istringstream& is);

  void command(const std::string& cmd);
}
