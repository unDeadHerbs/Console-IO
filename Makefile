CXX         = clang++
WARNINGS    = -Wall -Wextra -Wparentheses -Wno-dangling-else
LIBARYFLAGS = -lncurses -ltinfo
CXXFLAGS    = -std=c++1z $(WARNINGS) $(LIBARYFLAGS) -g $(SANS)

.PHONY:all seg msan
all: format TAGS deps mains
seg: clean msan
msan:
	make --no-print-directory all SANS=-fsanitize=address

# generate the etags file
TAGS:
	@rm -f TAGS
	@git ls-files|grep "pp$$"|xargs -r etags -a
	@echo "Generated Tags"

# use the etags file to find all excicutables
.PHONY:mains
mains:
	@for f in `ls *.c*` ; do \
		if etags $$f -o - | grep "int main(" - > /dev/null; \
			then echo $$f | sed -e 's/[.][^.]*$$/.bin/' -e 's/.*/make --no-print-directory &/' |sh; \
		fi ; \
	done

.PHONY:deps
deps:
	-@for f in `ls *.cpp` ; do \
		echo $$f | sed -e 's,cpp$$,d,' -e 's/.*/make -s .d\/&/'|sh; \
	done

# dependancy making
DEPDIR      = .d
$(shell mkdir -p $(DEPDIR) > /dev/null)
.PRECIOUS: $(DEPDIR)/%.d
$(DEPDIR)/%.d: %.cpp
	@set -e; rm -f $@; \
	 $(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
	 echo ".INTERMEDIATE: $*.o" >> $@; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ >> $@; \
	 sed -e 's,[.]o\([ :]\),.bin\1,g' -e 's,[.][hc]pp\>,.o,g' -e 's,\<[a-zA-Z]*[.]tpp\>,,g' < $@.$$$$ >> $@; \
	 printf '\t$$(CXX) $$(CXXFLAGS) $$(LIBARYFLAGS) -o $$@ $$^' >> $@; \
	 echo >> $@; \
	 rm -f $@.$$$$
	@echo "remade $@"

# emacs flycheck-mode
.PHONY:check-syntax csyntax
check-syntax: csyntax
csyntax:
	$(CXX) $(CXXFLAGS) -c ${CHK_SOURCES} -o /dev/null

.PHONY: clean
clean:
	rm -rf -- *.o *.bin .d/

.PHONY: format
format:
	@find|egrep '.*[.](cpp|hpp|cxx|hxx|cc|hc|c++|h++|c|h|tpp|txx)$$'|sed 's/[] ()'\''\\[&;]/\\&/g'|xargs clang-format -i
	@echo "reformatted code"


.PHONY: spell
spell:
	@echo " - Searching for Non-words in Code files -"
	@git ls | cut -f 2 | egrep "[.][ch](pp|)$$" | \
	 while read f; do \
		w=`cat $$f | aspell list --camel-case | sort | uniq | awk '{ if(length($$1)>4)print $$1 } '` ; \
		[ -z "$$w" ] || echo "$$f :: $$w" | xargs ; \
	 done

# This should be remade into a clang tool that removes strings
.PHONY: histogram
histogram:
	@echo " - Building a histogram of used words - "
	@git ls | cut -f 2 | egrep "[.][ch](pp|)$$" | \
	 while read f; do cat $$f; done | \
	 sed 's/\(\s\|[]({,<=>;})[*/+-]\|[0-9]\)/\n/g'|grep -v "^$"|\
	 sort|uniq -c|sort -gr|head

include $(wildcard $(DEPDIR)/*.d)
include $(wildcard *.d)
