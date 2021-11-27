#include "snek_game.hpp"
#include <tuple> // for std::ignore

using Direction = Snek::Direction;

class SnekAI {
  Snek const &s;

public:
  SnekAI(Snek const &S) : s(S) {}
  Direction get_move() {
    std::ignore = s;
    return Direction::left;
  }
};

int main() {
  Snek s;
  SnekAI ai(s);
  while (s.Alive())
    s.move(ai.get_move());

  return 0;
}
