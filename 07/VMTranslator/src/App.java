import java.io.File;

//to run in command line:
//javac App.java
//javac -d ../classes *.java
//java App ../../add/Add.asm
//java -cp ../classes App ../../add/Add.asm


public class App {
    public static void main(String[] args) throws Exception {
        System.out.println("asdf.asm".substring("asdf.asm".length() - 3));
        System.out.println("zero one two".split(" ")[2]);
        String inFileName = args[0];

        Parser parser;
        CodeWriter codeWriter;
        
        File input = new File(inFileName);
        if (input.isDirectory()) {
            File[] files = input.listFiles();
            String outFileName = inFileName.substring(0, inFileName.length() - 2) + "asm";
            codeWriter = new CodeWriter(outFileName);

            for (File file : files) {

                String filename = file.getName();
                if (filename.substring(filename.length() - 2) == "vm") {
                    parser = new Parser(filename);
                    Execute(parser, codeWriter);
                }
            }
        } else {
            String outFileName = inFileName.substring(0, inFileName.length() - 2) + "asm";
            parser = new Parser(inFileName);
            codeWriter = new CodeWriter(outFileName);
            Execute(parser, codeWriter);
        }
    }

    public static void Execute(Parser parser, CodeWriter codeWriter) {
        System.out.println("execute block");

        if(parser.hasMoreCommands()) {
            parser.advance();
            if(parser.commandType() == "C_ARITHMETIC") {
                codeWriter.writeArithmetic(parser.arg1());
            } else if (parser.commandType() == "C_PUSH" || parser.commandType() == "C_POP") {
                codeWriter.writePushPop(parser.commandType(), parser.arg1(), parser.arg2());
            }
        } else {
            codeWriter.close();
        }
    }
}
