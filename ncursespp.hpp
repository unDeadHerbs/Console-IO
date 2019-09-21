#ifndef __NCURSESPP_HPP__
#define __NCURSESPP_HPP__

#include <memory>
#include <utility>
#include "frame.hpp"

/**
 * A small wrapper for ncurses.
 *
 * This class defines a singelton object for manageing the ncurses
 * library.
 */
class ncurses{
protected:
  /**
   * This is a pointer to a WINDOW object in ncurses, but the ncurses
   * libaray makes a bunch of globabl variables so i'm encapsulating
   * it in udh_frame.cpp where it won't bother anything else.
   */
  void* win;
  typedef unsigned int uint;
  uint screenRows,screenCols;
  frame display;

  /**
   * Anti-Copy Constructor
   *
   * Making a private copy constructor to prevent usage.
   */
  ncurses(ncurses const&)=delete;

  /**
   * Anti-Asignment Operator
   *
   * Making a prevent asignment operator to prevent usage.
   */
  ncurses operator=(ncurses const&)=delete;
  /**
   * Anti-Constructor
   *
   * Prevent users from constructing this class.
   */
  ncurses();
  /**
   * Keep a pointer to the Singleton.
   */
public:
  ~ncurses();
  static ncurses* Get(){
    static std::unique_ptr<ncurses> Instance;
    if(!Instance)
      Instance = std::unique_ptr<ncurses>(new ncurses);
    return Instance.get();
  }

  std::pair<uint,uint> cursor;
  
  frame& getFrame();

  /**
   * Actuly render the chars to the screen.
   */
  void refreshScreen();

  /**
   * re-check the size of the screen.
   */
  void screenResizedTriger(int code);

  /**
   * Read in one char from the user.
   */
  int getKey()const;

  std::pair<uint,uint> size()const;
};

#endif
