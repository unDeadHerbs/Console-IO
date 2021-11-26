#include "ioconsole.hpp"

#include <ncurses.h>

// TODO: Check if threading is enabled?
#include <mutex>

std::recursive_mutex lock_ncurses;

namespace udh {

NCursesConsole::NCursesConsole() {
  std::lock_guard g(lock_ncurses);
  initscr();
  cbreak();
  nonl();
  noecho();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  getmaxyx(stdscr, screenRows, screenCols);
  win = newwin(int(screenRows), int(screenCols), 0, 0); // check the bounds
  screenResizedTriger(0);
  cursor = {0, 0};
}

NCursesConsole::~NCursesConsole() {
  std::lock_guard g(lock_ncurses);
  endwin();
}

void NCursesConsole::refreshScreen() {
  f.resize(screenRows,{});
  std::lock_guard g(lock_ncurses);
  for (uint row = 0; row < screenRows; row++) {
    wmove((WINDOW *)win, int(row), 0); // Check bounds
    f[row].resize(int(screenCols),' ');
    waddstr((WINDOW *)win, f[row].c_str());
  }
  wmove((WINDOW *)win, int(cursor.first), int(cursor.second)); // Check bounds
  wrefresh((WINDOW *)win);
}

void NCursesConsole::screenResizedTriger(int /*TODO: code*/) {
  std::lock_guard g(lock_ncurses);
  getmaxyx((WINDOW *)win, screenRows, screenCols);
  // Currently this is already done by redrawing the screen.
  //f.resize(screenRows,{});
  //for (uint row = 0; row < screenRows; row++)
  //  f[row].resize(screenCols,' ');

  // Not refreshing the screen makes the single handler faster.  The
  // overarching application can deal with that.  The lock is already
  // a pretty expensive operation to have in here.
  // TODO: Defer this work so that there's no lock.
  //refreshScreen();
}

int NCursesConsole::getKey(bool blocking) const {
  // Not locking because this is the only read.  I think this is safe
  // for now.
  //std::lock_guard g(lock_ncurses);
  nodelay((WINDOW *)win, !blocking);
  return wgetch((WINDOW*)win);
}

std::pair<uint, uint> NCursesConsole::size() const {
  return std::make_pair(screenRows, screenCols);
}

int NCursesConsole::ostream_buffer::sync() {
  auto s = this->str();
  // auto &cio = *NCursesConsole::Get();
  for (auto &c : s) {
    if (c == '\n') {
      if (cio.cursor.first == cio.screenRows - 1)
        break;
      cio.cursor.first++;
      cio.cursor.second = 0;
    } else {
      if (cio.cursor.second == cio.screenCols - 1)
        continue;
      cio.f[cio.cursor.first][cio.cursor.second++] = c;
    }
  }
  cio.refreshScreen();
  this->str("");
  return 0;
}

std::ostream &NCursesConsole::ostream() {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
  static std::unique_ptr<ostream_buffer> buffer_instance;
  static std::unique_ptr<std::ostream> Instance;
#pragma clang diagnostic pop
  if (!Instance) {
    buffer_instance = std::unique_ptr<ostream_buffer>(new ostream_buffer);
    Instance =
        std::unique_ptr<std::ostream>(new std::ostream(&*buffer_instance));
  }
  return *Instance;
}

std::string &NCursesConsole::operator[](uint row) {
  std::lock_guard g(lock_ncurses);
  // Just a segfault guard, it's not really above's fault if our size is off.
  if(f.size()<screenRows) f.resize(screenRows);
  if(f[row].size()<screenCols) f[row].resize(screenCols);
  return f[row];
  // This will need to make a sub class that forwards the [] as a <<
  // so that the buffering isn't interfeared with.  Also, the lock
  // will probably want to be held by that sub object, this is
  // technically a race.
}

bool NCursesConsole::move_cursor(Cursor c){
  cursor=c;
  return true;
}

} // namespace udh
