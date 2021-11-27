all: format simple.run snek.run

CXX = clang++
LanguageVersion = -std=c++20 -fno-exceptions
Should_be_Default_Warnings = -Wuninitialized -Wshadow
Real_Warnings = -Wall -Wextra -Wmissing-field-initializers -Wformat=2
Smell_Warnings = -Wunused -Wno-padded
NoWarn = -Wno-dangling-else -Wno-c++98-compat -Wno-missing-prototypes -Wno-old-style-cast -Wno-unused-macros -Wno-comma -Wno-return-std-move
#SANITIZER= -g -fsanitize=address -ftrapv
COLOR = always

ProjectFlags= -ltinfo -lncurses -ftrapv

Warnings = $(Should_be_Default_Warnings) $(Real_Warnings)
Debugging = -Werror -Wfatal-errors -fdiagnostics-color=$(COLOR) $(SANITIZER) $(Smell_Warnings)
CXXFLAGS = $(LanguageVersion) $(Warnings) $(NoWarn) $(Debugging) $(ProjectFlags)

%.bin: %*.cpp ../NCursesConsole.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: %.run
%.run: %.bin
	./$<

.PHONY: format
format:
	@clang-format -i ../*.cpp ../*.hpp *.cpp *.hpp
	@echo "reformatted code"
