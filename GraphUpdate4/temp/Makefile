line.exe: line.o
	icpc -o line.exe line.o
line.o: line.cpp
	icpc -c -o line.o line.cpp
extend.exe: extend.o Point.o
	icpc -o extend.exe extend.o Point.o
Point.o: Point.cpp
	icpc -c -o Point.o Point.cpp
extend.o: extend.cpp
	icpc -c -o extend.o extend.cpp
clean:
	rm -f *.o extend.exe
