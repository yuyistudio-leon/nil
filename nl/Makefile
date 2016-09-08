GXXFLAG=-g -Wall
GXX=g++ $(GXXFLAG) 
CPP_SOURCE=main.cpp value.cpp gc.cpp vm.cpp code.cpp parser.cpp lib.cpp util.cpp
CPP_OBJS = $(patsubst %.cpp, %.o, $(CPP_SOURCE))


default:bin

.PHONY:bin
bin: source ly
	$(GXX) -o nl.exe $(CPP_OBJS) ../ly/yacc.o

ly:
	make -C ../ly

.PHONY: source
source: $(CPP_SOURCE)
	make clean
	$(GXX) -c $(CPP_SOURCE)	

.PHONY:clean
clean:
	rm -f *.o
	rm -f nl.exe
