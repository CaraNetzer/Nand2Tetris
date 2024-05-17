tests = \
	ComplexArrays	\
	# Average	\
	# Seven	\
	# ConvertToBin	\
	# Square	\
	# Pong	\
	${null}

CFLAGS = -Wall -Werror

OBJS = JackCompiler.o JackTokenizer.o CompilationEngine.o SymbolTable.o Token.o

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


