FLAGS = --std=gnu++11 -Wall -Wextra -Wfatal-errors

all: test_bin

#======================================================================================================================
%_bin: src/%.cpp
	$(CXX) $< -o $@ -I. $(FLAGS)

#======================================================================================================================
.PHONY: test clean format ready

test: test_bin
	rm -f *.profraw *.gcov *.gcda
	./test_bin

clean:
	rm -f *.o *_bin *.gcov *.gcno *.gcda *.profraw

format:
	clang-format -i src/*.cpp

ready:
	@make --no-print-directory format
	@make --no-print-directory all
	@make --no-print-directory test
	git status
