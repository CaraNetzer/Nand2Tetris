import Parser
import CodeWriter
import sys
import os

# python3 main.py ../../ProgramFlow/BasicLoop/BasicLoop.vm

def Execute(parser, codeWriter):

    for currentLine in parser.file:
        currentLine = currentLine.strip()

        # if the line is not empty or a comment
        if currentLine and currentLine[0] != "/":
            print("line: " + currentLine)
            parser.currentLine = currentLine
            commandType = parser.commandType()

            if commandType == "C_ARITHMETIC":
                codeWriter.writeArithmetic(parser.arg1())
            elif commandType == "C_PUSH" or commandType =="C_POP":
                codeWriter.writePushPop(commandType, parser.arg1(), parser.arg2())
            else:
                print("commandType error: " + currentLine + ", commandType = " + commandType)

    parser.file.close()
    codeWriter.file.close()


def main():
    inFilePath = sys.argv[1:][0]

    if os.path.isdir(inFilePath):
        files = os.listdir(inFilePath)

        inFileNameArray = inFilePath.split("/")
        fileName = inFileNameArray[-2]
        outFileName = fileName + ".asm"
        outFilePath = "../outFiles/" + outFileName

        for fileName in files:
            if fileName[-2:] == "vm":
                parser = Parser.Parser(inFilePath + fileName)
                codeWriter = CodeWriter.CodeWriter(outFilePath, fileName)
                Execute(parser, codeWriter)

    else:
        inFileNameArray = inFilePath.split("/")
        fileName = inFileNameArray[-1]
        outFileName = fileName[0:-2] + "asm"
        outFilePath = "../outFiles/" + outFileName

        parser = Parser.Parser(inFilePath)
        codeWriter = CodeWriter.CodeWriter(outFilePath, fileName)
        Execute(parser, codeWriter)


main()


