FLAGS = --std=gnu++11 -Wall -Wextra -Wfatal-errors

all: test_bin

#======================================================================================================================
%_bin: test/%.cpp
	$(CXX) $< -o $@ -I. $(FLAGS)

#======================================================================================================================
.PHONY: test clean format ready

test: test_bin
	./test_bin

clean:
	rm -f *.o *_bin *.gcov *.gcno *.gcda *.profraw

format:
	clang-format -i src/*.hpp test/*.cpp

ready:
	@echo "-- Formatting with clang-format..."
	@make format --no-print-directory
	@echo "\n-- Compiling if necessary..."
	@make --no-print-directory
	@echo "\n-- Launching test..."
	@make test --no-print-directory
	@echo "\n-- All done, git status is:"
	@git status
