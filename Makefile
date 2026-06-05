All: poutine

poutine: poutine.o task.o
	c++ poutine.o task.o -o poutine ${CXXFLAGS}

poutine.o: poutine.cpp task.hpp
	c++ -c poutine.cpp ${CXXFLAGS}

task.o: task.cpp task.hpp
	c++ -c task.cpp ${CXXFLAGS}

clean:
	rm -v *.o poutine
