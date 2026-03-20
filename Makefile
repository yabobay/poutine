All: poutine

CXXFLAGS_ = ${CXXFLAGS}

poutine: poutine.o task.o
	c++ poutine.o task.o -o poutine ${CXXFLAGS_}

poutine.o: poutine.cpp task.hpp
	c++ -c poutine.cpp ${CXXFLAGS_}

task.o: task.cpp task.hpp
	c++ -c task.cpp ${CXXFLAGS_}

clean:
	rm -v *.o poutine
