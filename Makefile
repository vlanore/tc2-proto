FLAGS = --std=gnu++11 -Wall -Wextra -Wfatal-errors

all: test_bin

#======================================================================================================================
%_bin: test/%.cpp src/*.hpp
	$(CXX) $< -o $@ -I. $(FLAGS)

#======================================================================================================================
.PHONY: test clean format ready

test: test_bin
	./$@_bin

all_tests: all_tests_bin
	./$@_bin

clean:
	rm -f *.o *_bin *.gcov *.gcno *.gcda *.profraw

format:
	clang-format -i src/*.hpp test/*.cpp

ready:
	@echo "\e[34m===============================================================================\n>>\e[1m Formatting with clang-format... \e[0m\e[34m<<\e[0m"
	@make format --no-print-directory
	@echo "\n\e[34m===============================================================================\n>>\e[1m Compiling if necessary... \e[0m\e[34m<<\e[0m"
	@make --no-print-directory
	@echo "\n\e[34m===============================================================================\n>>\e[1m Launching tests... \e[0m\e[34m<<\e[0m"
	@make all_tests --no-print-directory
	@echo "\n\e[34m===============================================================================\n>>\e[1m All done, displaying git status \e[0m\e[34m<<\e[0m"
	@git status
