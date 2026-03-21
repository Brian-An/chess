CXX = g++-14
CXXFLAGS = -std=c++20 -Wall -MMD -Werror=vla # optional debugging flags: -g -fsanitize=address -fno-omit-frame-pointer -O0 -Wextra -pedantic
EXEC = chess
OBJECTS = util.o moveRequest.o bishop.o computerl1.o computerl2.o computerl3.o computerl4.o decisiontree.o graphicdisplay.o grid.o human.o king.o knight.o main.o openingExplorer.o pawn.o piece.o player.o queen.o rook.o subject.o textdisplay.o window.o 
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11 

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
