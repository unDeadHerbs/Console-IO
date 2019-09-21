#ifndef __FRAME_HPP__
#define __FRAME_HPP__

#include <deque>
#include <string>

/**
 * A small wrapper for ncurses.
 *
 * This class defines a singelton object for manageing the ncurses
 * library.
 */
class frame{
private:
  std::deque<std::string> f;
public:
  /**
   * Default constructer
   *
   * This does nothing as deque will already do the right thing.
   */
  frame(){}
  /**
   * Casting constructor from char
   *
   * Just so that memerb fucntions don't need to be duplicated.
   */
  frame(char);

  /**
   * size constructor
   *
   * Create a frame of the given size.
   */
  frame(uint);
  /**
   * Casting constructor from string.
   *
   * Just so that memerb fucntions don't need to be duplicated.
   */
  frame(std::string);
  
  std::string& operator[](uint rhs);
  char& operator[](std::pair<uint,uint> rhs);
  void add_row(std::string row);
  std::pair<uint,uint> size()const;
  void resize(uint rows,uint cols);
  void drop_row();
  void place(frame sub_frame,int row=0,int col=0);
  void place(frame sub_frame,std::pair<int,int> pos);
  frame clip(uint from_row,uint from_col,uint rows,uint cols);
  void clear();
};

#endif
