OSFLAG :=
ifeq ($(OS), Windows_NT)
	test-executable = test.exe
	test-open = start
else
	test-executable = test
	test-open =
endif

.PHONY: test test-compile build-examples

test: ./test/$(test-executable)
	$(info Test output will be written to ./test/test_output.txt)
	cd test && $(test-open) $(test-executable) --durations yes --out test_output.txt

./test/$(test-executable): ./test/test_main.o
	g++ -std=c++17 ./test/test_main.o ./test/test_tui.cpp -o ./test/test

./test/test_main.o: 
	g++ ./test/test_main.cpp -c -o ./test/test_main.o

test-compile:
ifeq (,$(wildcard ./test/test_main.o))
	$(info Compiling ./test/test_main.cpp, this only needs to be done once.)
	g++ ./test/test_main.cpp -c -o ./test/test_main.o
	g++ -std=c++17 ./test/test_main.o ./test/test_tui.cpp -o ./test/test
else
	g++ -std=c++17 ./test/test_main.o ./test/test_tui.cpp -o ./test/test
endif

build-examples:
	g++ ./examples/bar_chart.cpp      -o ./examples/bar_chart
	g++ ./examples/hello_world.cpp    -o ./examples/hello_world
	g++ ./examples/list.cpp           -o ./examples/list
	g++ ./examples/paragraph.cpp      -o ./examples/paragraph
