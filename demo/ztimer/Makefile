EXE_OUT := demo_ztimer.out
PATH_INC := ./ ../../inc
PATH_LIB := ../../lib
LIST_LIB :=

all:
	g++ -o $(EXE_OUT) *.cpp $(PATH_INC:%=-I%) $(PATH_LIB:%=-L%) $(LIST_LIB:%=-l%)
clean:
	rm $(EXE_OUT) *.o
test:
	./$(EXE_OUT)
