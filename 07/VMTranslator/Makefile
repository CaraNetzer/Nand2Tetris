# Set JAVA_DIR to java's bin directory on your machine 
# JAVA_DIR=/mnt/c/Program\ Files/Java/jdk-21/bin/
JAVAC=${JAVA_DIR}/javac.exe
JAVA=${JAVA_DIR}/java.exe

CP=classes/project7

CLASSES=	${CP}/App.class	\
		${CP}/CodeWriter.class	\
		${CP}/Parser.class	\
		${NULL}

# default: ../StackArithmetic/SimpleAdd/SimpleAdd.asm ../StackArithmetic/StackTest/StackTest.asm
default: check_java_dir $(patsubst %.vm,%.asm,$(wildcard ../*/*/*.vm))
	ls -l ../*/*/*.asm

check_java_dir: 
	@test \! -z "${JAVA_DIR}" || echo "\nPlease set JAVA_DIR in Makefile or as an Environment Variable\n"
	@test \! -z "${JAVA_DIR}"

%.asm: %.vm ${CLASSES}
	${JAVA} -cp classes project7.App $<

${CP}/%.class: src/%.java
	${JAVAC} -d classes -cp classes $<

${CP}/App.class: ${CP}/CodeWriter.class ${CP}/Parser.class

# javac -d ../classes *.java
# java -cp ../classes App ../../StackArithmetic/SimpleAdd/SimpleAdd.vm
