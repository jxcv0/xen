CFILES = src/cube.c src/xen.c src/glad.c src/input_sys.c
LIBS = -ldl -lm -lglfw -lpthread -lassimp

debug: mkbin
	@gcc -o bin/cube $(CFILES) -I src/ -Wall -Werror -o bin/cube -ggdb $(LIBS) -D XEN_DEBUG

release: mkbin
	@gcc -o bin/cube $(CFILES) -I src/ -Wall -Werror -o bin/cube -O3 $(LIBS)

mkbin:
	@if [ ! -d "bin" ]; then mkdir bin; fi

mktests: mkbin
	@if [ ! -d "bin/tests" ]; then mkdir bin/tests; fi

build_tests: mktests tests/lm_tests.c src/xen.c src/glad.c
	@gcc tests/lm_tests.c $(CFILES) -I src/ -o bin/tests/lm_tests -ggdb -ldl -lm -lglfw -lpthread -lassimp

run_tests: build_tests
	@./bin/tests/lm_tests

run:
	@./bin/cube

clean:
	@rm -r bin/*
