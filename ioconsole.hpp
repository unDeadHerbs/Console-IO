#ifndef IOCONSOLE_HPP
#define IOCONSOLE_HPP

// Following along with c++/ostream.

// I'm leaving majory portions commented out untill i both know what
// they are for and find them useful.

#include <ios>
#include <sstream>

namespace udh {
struct Cursor {
  // Left signed for ordinal optimizations.
  int row, col;
  Cursor() = default;
  Cursor(int r, int c) : row(r), col(c) {}
  auto operator<=>(Cursor const &) const = default;
};
template <class _CharT, class _Traits = std::char_traits<_CharT>>
class basic_console { //: virtual public basic_ios<_CharT,_Traits>{
public:
  // Types
  typedef _CharT char_type;
  typedef _Traits traits_type;
  typedef typename traits_type::int_type int_type;
  typedef typename traits_type::pos_type pos_type;
  typedef typename traits_type::off_type off_type;

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
  // virtual std::istream &istream() = 0;

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
  friend basic_console &operator<<(basic_console &lhs,
                                   basic_console &(*rhs)(basic_console &)) {
    return rhs(lhs);
  }
  
  template <typename T>
  friend basic_console &operator>>(basic_console &lhs, T &rhs) {
    lhs.istream() >> rhs;
    return lhs;
  }

  virtual bool move_cursor(Cursor) = 0;

protected:
  basic_console() {} // Do nothing, as this is for extensions.
};
typedef basic_console<char> console;
} // namespace udh
#endif
