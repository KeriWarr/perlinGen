CXX=g++
XXFLAGS=-Wall -MMD
EXEC=main
OBJECTS=main.o noiselayer.o randmath.o vector.o
DEPENDS=${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${XXFLAGS} ${OBJECTS} -o ${EXEC}
-include ${DEPENDS}

clean:
	rm ${OBJECTS}
