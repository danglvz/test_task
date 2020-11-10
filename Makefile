all: tests

create_lib:
		g++ -std=c++17 -c -o output_lib.o task.cpp
		ar rcs task_lib.a output_lib.o

tests: create_lib
		g++ -std=c++17 -o task_tests some_tests.cpp task_lib.a
