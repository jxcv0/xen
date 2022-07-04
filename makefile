CFILES = src/xen.c src/glad.c src/input_sys.c
LIBS = -ldl -lm -lglfw -lpthread -lassimp
TESTS = tests/lm_tests.c tests/input_sys_tests.c

debug: mkbin $(CFILES)
	@gcc -o bin/cube $(CFILES) src/game.c -I src/ -Wall -Werror -o bin/cube -ggdb $(LIBS) -D XEN_DEBUG

release: mkbin $(CFILES)
	@gcc -o bin/cube $(CFILES) srx/game.c -I src/ -Wall -Werror -o bin/cube -O3 $(LIBS)

mkbin:
	@if [ ! -d "bin" ]; then mkdir bin; fi

mktests: mkbin
	@if [ ! -d "bin/tests" ]; then mkdir bin/tests; fi

tests: mktests $(TESTS) $(CFILES)
	@gcc tests/lm_tests.c $(CFILES) -I src/ -o bin/tests/lm_tests -ggdb $(LIBS)
	@gcc tests/input_sys_tests.c $(CFILES) -I src/ -o bin/tests/input_sys_tests -ggdb $(LIBS)

test: tests
	@./bin/tests/lm_tests

run:
	@./bin/cube

clean:
	@rm -r bin/*
