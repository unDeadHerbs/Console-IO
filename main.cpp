#include "ncursespp.hpp"
#include <unistd.h>

int main() {
  sleep(1);
  auto screen = ncurses::Get();
  screen->refreshScreen();
  sleep(1);
  screen->getFrame()[1][1] = 'a';
  screen->refreshScreen();
  sleep(1);
}
