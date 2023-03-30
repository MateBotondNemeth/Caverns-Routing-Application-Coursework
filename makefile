cavern_gcc:
	gcc -o cave.obj -c cave.cpp
	ar rcs cave.lib cave.obj
    g++ option_a.cpp cave.lib -o caverns

cavern_cl:
	cl option_a.cpp cave.cpp

clean:
    del *.obj
	del *.exe
	del *.lib
	del *.txt