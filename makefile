mkbin:
	@if [ ! -d "bin" ]; then mkdir bin; fi

mktests: mkbin
	@if [ ! -d "bin/tests" ]; then mkdir bin/tests; fi

build_tests: mktests tests/lm_tests.c src/xen.c src/glad.c
	@gcc tests/lm_tests.c src/xen.c src/glad.c -I src/ -o bin/tests/lm_tests -ggdb -ldl -lm -lglfw -lpthread -lassimp

run_tests: build_tests
	@./bin/tests/lm_tests

build: mkbin
	@gcc game/cube.c src/xen.c src/glad.c -I src/ -Wall -Werror -o bin/cube -ggdb -ldl -lm -lglfw -lpthread -lassimp

run:
	@./bin/cube

clean:
	@rm -r bin/*
