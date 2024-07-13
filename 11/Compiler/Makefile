tests = \
	ComplexArrays	\
	Pong	\
	Square	\
	Seven	\
	ConvertToBin	\
	Average	\
	${null}

CFLAGS = -Wall -Werror

# OBJS = JackCompiler.o JackTokenizer.o CompilationEngine.o SymbolTable.o Token.o VMWriter.o CompilationIntestines.o CompileStatements.o
OBJS = $(patsubst %.c,%.o,$(wildcard *.c))

default: test

test: JackCompiler
	for test in ${tests}; do \
		./JackCompiler ../$$test/; \
	done

JackCompiler: ${OBJS}

${OBJS}: Token.h


clean:
	rm -f ${OBJS}

clobber: clean
	rm -f JackCompiler


