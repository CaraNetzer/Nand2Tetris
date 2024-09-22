files = CodeWriter.py Parser.py main.py Makefile 

default: VMTranslator

VMTranslator: Makefile
	echo "#!/bin/sh" > VMTranslator
	echo >> VMTranslator
	echo 'python3 main.py "$$@"' >> VMTranslator
	chmod +x VMTranslator

zip: project08.zip

project08.zip: ${files}
	zip $@ ${files}
