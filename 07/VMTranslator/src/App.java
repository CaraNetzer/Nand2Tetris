import java.io.File;
import java.io.IOException;

//to run in command line:
//javac App.java
//javac -d ../classes *.java
//java App ../../StackArithmetic/SimpleAdd/SimpleAdd.vm
//java -cp ../classes App ../../StackArithmetic/SimpleAdd/SimpleAdd.vm


public class App {
    public static void main(String[] args) throws Exception {
        
        String inFileName = args[0];

        Parser parser;
        CodeWriter codeWriter;
        
        File input = new File(inFileName);
        if (input.isDirectory()) {
            File[] files = input.listFiles();

            for (File file : files) {
                String filename = file.getName();
                if (filename.substring(filename.length() - 2).equals("vm")) {

                    String outFileName = filename.substring(0, inFileName.length() - 2) + "asm";
                    codeWriter = new CodeWriter(outFileName);

                    parser = new Parser(filename);
                    Execute(parser, codeWriter);
                }
            }
        } else {

            String[] inFileNameArray = inFileName.split("/");
            String filename = inFileNameArray[inFileNameArray.length - 1];
            String outFileName = filename.substring(0, filename.length() - 2) + "asm";
            parser = new Parser(inFileName);
            codeWriter = new CodeWriter(outFileName);
            Execute(parser, codeWriter);
        }
    }

    public static void Execute(Parser parser, CodeWriter codeWriter) throws IOException {
        
        while(parser.currentLine != null) {
            parser.advance();
            if(parser.hasMoreCommands()) {
                if(parser.currentLine.length() == 0 || 
                   parser.currentLine.substring(0,1).equals("/")) {
                    parser.advance();
                } else {
                    String commandType = parser.commandType();
                    if(commandType.equals("C_ARITHMETIC")) {
                        codeWriter.writeArithmetic(parser.arg1());
                    } else if (commandType.equals("C_PUSH") || commandType.equals("C_POP")) {
                        codeWriter.writePushPop(commandType, parser.arg1(), parser.arg2());
                    } else {
                        //System.out.println("error App line 60: commandType = " + commandType);
                    }
                }            
            }
        } 

        codeWriter.close();
        
    }
}
