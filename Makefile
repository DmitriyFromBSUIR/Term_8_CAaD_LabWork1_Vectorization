all:
	icc -fasm-blocks -O3 -qopt-report2 main.cpp
	./a.out
