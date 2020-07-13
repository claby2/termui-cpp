OSFLAG :=
ifeq ($(OS), Windows_NT)
	test-executable = test.exe
	test-open = start
	ncurses-flag = 
else
	test-executable = test
	test-open =
	ncurses-flag = -lncurses
endif

.PHONY: test test-compile build-examples

test: ./test/$(test-executable)
	$(info Test output will be written to ./test/test_output.txt)
	cd test && $(test-open) $(test-executable) --durations yes --out test_output.txt

./test/$(test-executable): ./test/test_main.o
	g++ -std=c++17 ./test/test_main.o ./test/test_tui.cpp $(ncurses-flag) -o ./test/test

./test/test_main.o: 
	g++ ./test/test_main.cpp -c -o ./test/test_main.o

test-compile:
ifeq (,$(wildcard ./test/test_main.o))
	$(info Compiling ./test/test_main.cpp, this only needs to be done once.)
	g++ ./test/test_main.cpp -c -o ./test/test_main.o
	g++ -std=c++17 ./test/test_main.o ./test/test_tui.cpp $(ncurses-flag) -o ./test/test
else
	g++ -std=c++17 ./test/test_main.o ./test/test_tui.cpp $(ncurses-flag) -o ./test/test
endif

build-examples:
	g++ -std=c++17 ./examples/bar_chart.cpp   $(ncurses-flag) -o ./examples/bar_chart
	g++ -std=c++17 ./examples/gauge.cpp       $(ncurses-flag) -o ./examples/gauge
	g++ -std=c++17 ./examples/hello_world.cpp $(ncurses-flag) -o ./examples/hello_world
	g++ -std=c++17 ./examples/list.cpp        $(ncurses-flag) -o ./examples/list
	g++ -std=c++17 ./examples/paragraph.cpp   $(ncurses-flag) -o ./examples/paragraph
