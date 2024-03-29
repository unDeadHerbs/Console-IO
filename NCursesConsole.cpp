#include "NCursesConsole.hpp"

#include <ncurses.h>

// TODO: Check if threading is enabled?
#include <mutex>

std::recursive_mutex lock_ncurses;

#define COLOUR_NUMBER(FG, BG) (FG << 4 | BG)

constexpr auto ncurses_color_names(int co) {
  // BGR colours (same as RGB but backwards).
  switch (0b111 & co) {
  case 0b000:
    return COLOR_BLACK;
  case 0b001:
    return COLOR_RED;
  case 0b010:
    return COLOR_GREEN;
  case 0b011:
    return COLOR_YELLOW;
  case 0b100:
    return COLOR_BLUE;
  case 0b101:
    return COLOR_MAGENTA;
  case 0b110:
    return COLOR_CYAN;
  case 0b111:
    return COLOR_WHITE;
  default:
    __builtin_unreachable();
  }
}

void setup_colours() {
  for (int fg = 0b000; fg <= 0b111; fg++)
    for (int bg = 0b000; bg <= 0b111; bg++)
      init_pair(COLOUR_NUMBER(fg, bg), ncurses_color_names(fg),
                ncurses_color_names(bg));
}

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
  // win = newwin(int(screenRows), int(screenCols), 0, 0);
  screenResizedTriger(0);
  cursor = {0, 0};
  if (has_colors()) {
    start_color();
    setup_colours();
  }
}

NCursesConsole::~NCursesConsole() {
  std::lock_guard g(lock_ncurses);
  endwin();
}

void NCursesConsole::refreshScreen() {
  f.resize(LINES, {});
  std::lock_guard g(lock_ncurses);
  for (int row = 0; row < LINES; row++) {
    f[row].resize(COLS, ' ');
    move(row, 0);
    if (has_colors())
      // TODO: This is very inefficient, try to check for changes and
      // for if there are ranges of color that don't need toggling.
      for (int col = 0; col < COLS; col++) {
        auto const &c = f[row][col];
        attron(COLOR_PAIR(COLOUR_NUMBER(c.fg, c.bg)));
        addch(c);
        attroff(COLOR_PAIR(COLOUR_NUMBER(c.fg, c.bg)));
      }
    else
      addstr(f[row].c_str());
  }
  move(int(cursor.row), int(cursor.col)); // Check bounds
  refresh();
}

void NCursesConsole::screenResizedTriger(int /*TODO: code*/) {
  std::lock_guard g(lock_ncurses);

  // Currently this is also done by redrawing the screen.
  f.resize(LINES, {});
  for (int row = 0; row < LINES; row++)
    f[row].resize(COLS, ' ');
}

int NCursesConsole::getKey(bool blocking) const {
  // Not locking because this is the only read.  I think this is safe
  // for now.
  // std::lock_guard g(lock_ncurses);
  // nodelay((WINDOW *)win, !blocking);
  nodelay(stdscr, !blocking);
  return getch();
}

NCursesConsole::ScreenSize NCursesConsole::size() const {
  return {LINES, COLS};
}

void NCursesConsole::addchar(char c) {
  std::lock_guard g(lock_ncurses);
  if (c == '\n') {
    if (cursor.row >= LINES)
      return;
    cursor.row++;
    cursor.col = 0;
  } else {
    if (cursor.col >= COLS or cursor.row >= LINES)
      return;
    f[cio.cursor.row].set(cio.cursor.col++, c);
  }
}

int NCursesConsole::ostream_buffer::sync() {
  auto s = this->str();
  // auto &cio = *NCursesConsole::Get();
  for (auto &c : s)
    cio.addchar(c);
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
  // TODO: take a once lock here to prevent double initializing.
  if (!Instance) {
    buffer_instance = std::unique_ptr<ostream_buffer>(new ostream_buffer);
    Instance =
        std::unique_ptr<std::ostream>(new std::ostream(&*buffer_instance));
  }
  return *Instance;
}

colour_string &NCursesConsole::operator[](int row) {
  std::lock_guard g(lock_ncurses);
  // Just a segfault guard, it's not really above's fault if our size is off.
  if (int(f.size()) < LINES)
    f.resize(LINES);
  if (int(f[row].size()) < COLS)
    f[row].resize(COLS);
  return f[row];
  // This will need to make a sub class that forwards the [] as a <<
  // so that the buffering isn't interfeared with.  Also, the lock
  // will probably want to be held by that sub object, this is
  // technically a race.
}

bool NCursesConsole::move_cursor(Cursor c) {
  cursor = c;
  return true;
}

} // namespace udh
