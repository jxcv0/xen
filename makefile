CFILES = src/xen.c src/glad.c src/input_sys.c src/logger.c src/resources.c
FLAGS = -Wall -Werror
LIBS = -ldl -lm -lglfw -lpthread -lassimp
TESTS = tests/maths_tests.c tests/input_sys_tests.c tests/resources_tests.c

debug: mkbin $(CFILES)
	@gcc -o bin/cube $(CFILES) src/game.c -I src/ $(FLAGS) -o bin/cube -ggdb $(LIBS) -D XEN_DEBUG

release: mkbin $(CFILES)
	@gcc -o bin/cube $(CFILES) src/game.c -I src/ $(FLAGS) -o bin/cube -O3 $(LIBS)

tests: mktests $(TESTS) $(CFILES)
	@gcc tests/maths_tests.c $(CFILES) -I src/ $(FLAGS) -o bin/tests/lm_tests -ggdb $(LIBS) -D XEN_DEBUG
	@gcc tests/input_sys_tests.c $(CFILES) -I src/ $(FLAGS) -o bin/tests/input_sys_tests -ggdb $(LIBS) -D XEN_DEBUG
	@gcc tests/resources_tests.c $(CFILES) -I src/ $(FLAGS) -o bin/tests/resources_tests -ggdb $(LIBS) -D XEN_DEBUG

test: tests
	@./bin/tests/lm_tests
	@./bin/tests/input_sys_tests
	@./bin/tests/resources_tests

run:
	@./bin/cube

clean:
	@rm -r bin/*

mkbin:
	@if [ ! -d "bin" ]; then mkdir bin; fi

mktests: mkbin
	@if [ ! -d "bin/tests" ]; then mkdir bin/tests; fi

