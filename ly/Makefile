GXXFLAG=-g
GXX=g++ $(GXXFLAG) 

default:lex_yacc

.PHONY:lex
lex: lex.l.cpp 
	flex -o lex.c $^

.PHONY:yacc
yacc: yacc.y.cpp
	yacc -v -o yacc.cpp -d $^

.PHONY:lex_yacc
lex_yacc: lex yacc
	$(GXX) -c -o yacc.o yacc.cpp

.PHONY:clean
clean:
	rm -f *.o
	rm -f yacc.cpp
	rm -f yacc.hpp
	rm -f lex.c
	rm bin
