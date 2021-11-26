#ifndef IOCONSOLE_HPP
#define IOCONSOLE_HPP

// Following along with c++/ostream.

// I'm leaving majory portions commented out untill i both know what
// they are for and find them useful.

#include <deque>
#include <ios>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <utility>

namespace udh {
template <class _CharT, class _Traits = std::char_traits<_CharT>>
class basic_console { //: virtual public basic_ios<_CharT,_Traits>{
public:
  // Types
  typedef _CharT char_type;
  typedef _Traits traits_type;
  typedef typename traits_type::int_type int_type;
  typedef typename traits_type::pos_type pos_type;
  typedef typename traits_type::off_type off_type;
  typedef std::pair<int, int> Cursor; // Left signed for ordinal optimizations.

  /*
    explicit basic_console(basic_streambuf<char_type,traits_type>* __sb)
    { this->init(_sb); }
  */
  virtual ~basic_console() {}

protected:
  /*
    I think both of thses are for c++03

    Do I need a move constructor?
    basic_console(basic_console&& __rhs);

    They have a `27.7.2.3 Assign/swap`?
  */

  /*
    void swap(basic_console& __rhs);
  */

  // Both of these are to be deleted in cio?
  basic_console(const basic_console &__rhs);
  basic_console &operator=(const basic_console &__rhs);

private:
  virtual std::ostream &ostream() = 0;
  //virtual std::istream &istream() = 0;

public:
  template <typename T>
  friend basic_console &operator<<(basic_console &lhs, T rhs) {
    lhs.ostream() << rhs;
    return lhs;
  }
  // TODO: There's probably a more generic/better version of this.
  friend basic_console &operator<<(basic_console &lhs,
                                   std::ostream &(*rhs)(std::ostream &)) {
    rhs(lhs.ostream());
    return lhs;
  }

  template <typename T>
  friend basic_console &operator>>(basic_console &lhs, T &rhs) {
    lhs.istream() >> rhs;
    return lhs;
  }

  virtual bool move_cursor(Cursor)=0;

protected:
  basic_console() {} // Do nothing, as this is for extensions.
};
typedef basic_console<char> console;

// TODO: Specialize on number of colours supported with an "UNKNOWN"
// option.
struct colour_char{
  enum COLOUR{
    BLACK=0b000,
    RED=0b001,
    GREEN=0b010,
    YELLOW=0b011,
    BLUE=0b100,
    MAGENTA=0b101,
    CYAN=0b110,
    WHITE=0b111};
  // GRAY ,BRIGHT_RED,BRIGHT_GREEN,BRIGHT_YELLOW,BRIGHT_BLUE,BRIGHT_MAGENTA,BRIGHT_CYAN,BRIGHT_WHITE};
  char c;
  int bg;
  int fg;
  colour_char()=default;
  colour_char(char ch,COLOUR fg_=WHITE):c(ch),bg(COLOUR::BLACK),fg(fg_){}
  colour_char& operator=(char ch){c=ch; return *this;}
  operator char()const{return c;}
};

struct slice_range{
  int start;
  int end;
  int step=1;
  slice_range& scope(int const size){
    if(start<0)start+=size;
    if(end<0)end+=size;
    return *this;
  }
};
  
class colour_string{
  /// The underlying container.
  std::basic_string<colour_char> s;
  /// A normal string for speed.
  std::string _str;
  colour_string(std::basic_string<colour_char> cs,std::string str):s(cs),_str(str){}
public:
  colour_string()=default;
  colour_string(std::string str,std::optional<colour_char::COLOUR> fg):_str(str){
    s.reserve(str.size());
    if(fg)
      for(auto const & c:str){
        s.push_back(c);
        s.rbegin()->fg=*fg;
      }
    else
      for(auto const & c:str)
        s.push_back(c);
  }
  colour_string(std::string str):_str(str){
    s.reserve(str.size());
    for(auto const & c:str)s.push_back(c);
  }
  auto size()const{return _str.size();}
  auto c_str()const{
    return _str.c_str();
  }
  colour_string operator+=(colour_string const & rhs){
    s+=rhs.s;
    _str+=rhs._str;
    return *this;
  }
  colour_string operator+(colour_string const & rhs)const{
    return {s+rhs.s,_str+rhs._str};  
}
  colour_string& operator+=(colour_char const & rhs){
    s+=rhs;
    _str+=rhs;
    return *this;
  }
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wignored-qualifiers"
  [[nodiscard]] auto const operator[](int pos){
 #pragma clang diagnostic pop
    // Enable python style reverse indexing.
    if(pos<0)
      pos+=s.size();
    // TODO: Const until can write to both containers.
    return s[pos];
  }
  [[nodiscard]] colour_string const operator[](slice_range range){
    // TODO: return type const so that it can later hand back a
    // reference to a mutable sub-string.
    range.scope(_str.size());
    // TODO: range.step
    return {s.substr(range.start,range.end),
            _str.substr(range.start,range.end)};
  }
  colour_string& set(int pos,colour_char val){
    s[pos]=val;
    _str[pos]=val;
    return *this;
  }
  auto pop_back(){
    _str.pop_back();
    return s.pop_back();
  }
  inline constexpr void resize(size_t count, colour_char ch=' '){
    s.resize(count, ch);
    _str.resize(count,ch);
  }
  void colour(slice_range range,colour_char::COLOUR fg,std::optional<colour_char::COLOUR> bg={}){
    // TODO: range.step
    range.scope(_str.size());
    for(int p=range.start;p<=range.end;p++){
      s[p].fg=fg;
      if(bg)s[p].bg=*bg;
    }
  }
};
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
  //void *win;
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

  std::pair<uint, uint> size() const;

  colour_string& operator[](uint);
  class ostream_buffer : public std::stringbuf {
    virtual int sync();
  };
  friend class ostream_buffer;
  std::ostream& ostream();

  bool move_cursor(Cursor);
};
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
static auto &cio = *NCursesConsole::Get();
#pragma clang diagnostic pop
} // namespace udh
#endif
