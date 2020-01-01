#ifndef __IOCONSOLE_HPP__
#define __IOCONSOLE_HPP__

// Following along with c++/ostream.

// I'm leaving majory portions commented out untill i both know what
// they are for and find them useful.

#include <deque>
#include <ios>
#include <memory>
#include <string>
#include <utility>

namespace udh {
template <class _CharT, class _Traits = std::char_traits<_CharT>>
class basic_console {  //: virtual public basic_ios<_CharT,_Traits>{
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
	basic_console(const basic_console& __rhs);
	basic_console& operator=(const basic_console& __rhs);

 private:
	virtual std::ostream ostream() = 0;
	// virtual std::istream istream()=0;
 public:
	template <typename T>
	friend basic_console& operator<<(basic_console& lhs, T& rhs) {
		lhs.ostream << rhs;
		return lhs;
	}
	template <typename T>
	friend basic_console& operator>>(basic_console& lhs, T& rhs) {
		lhs.istream >> rhs;
		return lhs;
	}

 protected:
	basic_console() {}  // do nothing as we are for extension
};
typedef basic_console<char> console;

/**
 * A small wrapper for ncurses.
 *
 * This class defines a singelton object for manageing the ncurses
 * library.
 */
class NCursesConsole : public console {
 protected:
	/**
	 * This is a pointer to a WINDOW object in ncurses, but the ncurses
	 * libaray makes a bunch of globabl variables so i'm encapsulating
	 * it in udh_frame.cpp where it won't bother anything else.
	 */
	void* win;
	typedef unsigned int uint;
	uint screenRows, screenCols;
	std::deque<std::string> f;

	/**
	 * Anti-Copy Constructor
	 *
	 * Making a private copy constructor to prevent usage.
	 */
	NCursesConsole(NCursesConsole const&) = delete;

	/**
	 * Anti-Asignment Operator
	 *
	 * Making a prevent asignment operator to prevent usage.
	 */
	NCursesConsole operator=(NCursesConsole const&) = delete;
	/**
	 * Anti-Constructor
	 *
	 * Prevent users from constructing this class.
	 */
	NCursesConsole();

	std::pair<uint, uint> cursor;

 public:
	~NCursesConsole();
	static NCursesConsole* Get() {
		static std::unique_ptr<NCursesConsole> Instance;
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
	int getKey() const;

	std::pair<uint, uint> size() const;

	std::string& operator[](uint);
	class ostream_buffer : public std::stringbuf;
	std::ostream ostream();
};
static auto& cio = *NCursesConsole::Get();
};  // namespace udh
#endif
