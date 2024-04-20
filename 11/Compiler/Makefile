tests = \
	Seven	\
	# Average	\
	# ConvertToBin	\
	# Square	\
	# Pong	\
	# ComplexArrays	\
	${null}

CFLAGS = -Wall -Werror

default: test

test: JackCompiler
	for test in ${tests}; do \
		./JackCompiler ../$$test/; \
	done

JackCompiler: JackCompiler.o JackTokenizer.o
