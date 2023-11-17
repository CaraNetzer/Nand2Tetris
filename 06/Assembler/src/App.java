import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;

//to run in command line:
//javac App.java
//java App ../../add/Add.asm
public class App {
    public static void main(String[] args) throws Exception {
        String commandType, textSymbol, textDest, textComp, textJump; //parsed from Assembly code
        String binarySymbol, binaryDest, binaryComp, binaryJump, binaryInstruction; // translated to binary

        try {
            String filePath = args[0];
            String[] inFileNameArray = filePath.split("/");
            String inFileName = inFileNameArray[inFileNameArray.length - 1];
            String outFileName = inFileName.substring(0, inFileName.length() - 3) + "hack";

            File file = new File(outFileName);
            file.createNewFile();

            int currentLineNumber = 0;

            Parser parser = new Parser(filePath);
            Code translationCodes = new Code();
            try (BufferedWriter bw = new BufferedWriter(new FileWriter(outFileName));) {

                for (String line; (line = Parser.br.readLine()) != null; currentLineNumber++) {

                    if (!line.trim().isEmpty() && line.charAt(0) != '/') {
                        Parser.currentLine = line;
                        // System.out.println(line);
                        commandType = Parser.commandType();
                        if (commandType == "A_COMMAND" || commandType == "L_COMMAND") {
                            // #region read lines
                            textSymbol = parser.symbol();
                            // #endregion

                            // #region translate lines
                            int number = Integer.parseInt(textSymbol);
                            binarySymbol = "0" + String.format("%15s", Integer.toBinaryString(number)).replace(' ', '0');
                            // #endregion

                            // #region write lines
                            bw.write(binarySymbol + "\n");
                            // #endregion
                        }
                        if (commandType == "C_COMMAND") {
                            // #region read lines
                            textDest = parser.dest();
                            textComp = parser.comp();
                            textJump = parser.jump();
                            // System.out.println("dest: " + dest + "; comp: " + comp + "; jump: " + jump);
                            // #endregion

                            // #region translate lines
                            binaryDest = translationCodes.dest(textDest != null ? textDest : "null");
                            binaryComp = translationCodes.comp(textComp);
                            binaryJump = translationCodes.jump(textJump != null ? textJump : "null");
                            binaryInstruction = "111" + binaryComp + binaryDest + binaryJump;
                            // #endregion

                            // #region write lines
                            bw.write(binaryInstruction + "\n");
                            // #endregion
                        }
                    }

                }
            }

        } catch (

        Exception e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
    }
}