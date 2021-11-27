#ifndef __SNEK_CLASS_HPP__
#define __SNEK_CLASS_HPP__

#include <string>
#include <utility>
#include <vector>

#include "../NCursesConsole.hpp"
#include "../ioconsole.hpp"

class Snek {
  udh::NCursesConsole::ScreenSize size;
  std::vector<udh::Cursor> body; // [0] is head
  udh::Cursor food;

public:
  typedef enum { none, up, right, down, left } Direction;
  useconds_t sleep_time = 100000;

private:
  Direction direction;
  bool alive;

  void drawWalls();
  void updateDisplay();

public:
  Snek();
  bool Alive() { return alive; }
  bool move(Direction movement = none);
};

#endif
