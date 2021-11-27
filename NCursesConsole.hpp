#ifndef NCURSES_CONSOLE_HPP
#define NCURSES_CONSOLE_HPP

#include <deque>
#include <ios>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <utility>

#include "Color_Chars.hpp"
#include "ioconsole.hpp"

namespace udh {
/**
 * A small wrapper for ncurses.
 *
 * This class defines a singelton object for manageing the ncurses
 * library.
 */
class NCursesConsole : public basic_console<colour_char> {
protected:
  /**
   * This is a pointer to a WINDOW object in ncurses, but the ncurses
   * libaray makes a bunch of globabl variables so i'm encapsulating
   * it in udh_frame.cpp where it won't bother anything else.
   */
  // void *win;
  /// The internal container for the screen.
  std::deque<colour_string> f;

  /**
   * Anti-Copy Constructor
   *
   * Making a private copy constructor to prevent usage.
   */
  NCursesConsole(NCursesConsole const &) = delete;

  /**
   * Anti-Asignment Operator
   *
   * Making a prevent asignment operator to prevent usage.
   */
  NCursesConsole operator=(NCursesConsole const &) = delete;
  /**
   * Anti-Constructor
   *
   * Prevent users from constructing this class.
   */
  NCursesConsole();

  Cursor cursor;

  void addchar(char c);

public:
  ~NCursesConsole();
  static NCursesConsole *Get() {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
    static std::unique_ptr<NCursesConsole> Instance;
#pragma clang diagnostic pop
    if (!Instance)
      Instance = std::unique_ptr<NCursesConsole>(new NCursesConsole);
    return Instance.get();
  }

  // TODO: move cursor functions

  /**
   * Actuly render the chars to the screen.
   */
  void refreshScreen();

  /**
   * re-check the size of the screen.
   *
   * TODO: set up the handler for this.
   * TODO: have functions be adddable to this.
   */
  void screenResizedTriger(int code);

  /**
   * Read in one char from the user.
   */
  int getKey(bool blocking) const;

  struct ScreenSize {
    int rows, cols;
  };
  ScreenSize size() const;

  colour_string &operator[](int);
  class ostream_buffer : public std::stringbuf {
    virtual int sync();
  };
  friend class ostream_buffer;
  std::ostream &ostream();

  bool move_cursor(Cursor);
};
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
static auto &cio = *NCursesConsole::Get();
#pragma clang diagnostic pop
} // namespace udh
#endif
