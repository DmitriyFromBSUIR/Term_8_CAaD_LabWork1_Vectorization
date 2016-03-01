all:
	/home/dmitry/Installed_Soft/Intel/Intel_Parallel_Studio_XE_Composer_Edition_Update_3/bin/icc -fasm-blocks -O3 -qopt-report2 main.cpp
	./a.out
