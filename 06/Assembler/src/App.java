import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.util.Set;

//to run in command line:
//javac App.java
//javac -d ../classes *.java
//java App ../../add/Add.asm
//java -cp ../classes App ../../add/Add.asm
public class App {
    public static void main(String[] args) throws Exception {
        String commandType, textSymbol, textDest, textComp, textJump; // parsed from Assembly code
        String binarySymbol, binaryDest, binaryComp, binaryJump, binaryInstruction; // translated to binary
        int machineCodeLineNumber = 0;
        int maxRAMValue = 15;

        try {
            String filePath = args[0];
            String[] inFileNameArray = filePath.split("/");
            String inFileName = inFileNameArray[inFileNameArray.length - 1];
            String outFileName = inFileName.substring(0, inFileName.length() - 3) + "hack";

            int currentLineNumber = 0;

            Code translationCodes = new Code();
            SymbolTable symbolTable = new SymbolTable();

            Parser parserFirstPass = new Parser(filePath);
            for (String line; (line = parserFirstPass.br.readLine()) != null; currentLineNumber++) {
                if (!line.trim().isEmpty() && line.charAt(0) != '/') {
                    parserFirstPass.currentLine = line;
                    commandType = parserFirstPass.commandType();
                    if (commandType == "A_COMMAND" || commandType == "C_COMMAND") {
                        machineCodeLineNumber++;
                    } else if (commandType == "L_COMMAND") {
                        symbolTable.addEntry(parserFirstPass.symbol(), machineCodeLineNumber);
                    }
                }
                else if (line.contains("/")) {
                    line = line.substring(0, line.indexOf("/"));
                }
            }
            parserFirstPass.br.close();

            machineCodeLineNumber = 0;
            Parser parserSecondPass = new Parser(filePath);
            try (BufferedWriter bw = new BufferedWriter(new FileWriter("../outFiles/" + outFileName));) {
                for (String line; (line = parserSecondPass.br.readLine()) != null; currentLineNumber++) {
                    if (!line.trim().isEmpty() && line.charAt(0) != '/') {
                        parserSecondPass.currentLine = line;
                        commandType = parserSecondPass.commandType();

                        if (commandType == "A_COMMAND") {
                            // #region read lines
                            textSymbol = parserSecondPass.symbol();
                            // #endregion

                            // #region translate lines
                            int registerNumber = tryParseInt(textSymbol, -1);
                            if (registerNumber == -1) {
                                if (!symbolTable.contains(textSymbol)) {
                                    maxRAMValue++;
                                    symbolTable.addEntry(textSymbol, maxRAMValue);
                                }
                                int variableNameAddress = symbolTable.getAddress(textSymbol);

                                binarySymbol = "0" + String.format("%15s", Integer.toBinaryString(variableNameAddress))
                                        .replace(' ', '0');
                            } else {
                                binarySymbol = "0" + String.format("%15s", Integer.toBinaryString(registerNumber))
                                        .replace(' ', '0');
                            }
                            // #endregion

                            // #region write lines
                            bw.write(binarySymbol + "\n");
                            // #endregion

                            machineCodeLineNumber++;
                        }
                        if (commandType == "C_COMMAND") {
                            // #region read lines
                            textDest = parserSecondPass.dest();
                            textComp = parserSecondPass.comp();
                            textJump = parserSecondPass.jump();
                            // System.out.println("dest: " + textDest + "; comp: " + textComp + "; jump: " + textJump);
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

                            machineCodeLineNumber++;
                        }
                    }
                }
            }
            parserSecondPass.br.close();

        } catch (Exception e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
    }

    public static int tryParseInt(String value, int defaultVal) {
        try {
            return Integer.parseInt(value);
        } catch (NumberFormatException e) {
            return defaultVal;
        }
    }
}

// Set<String> keys = symbolTable.keySet();
// for(String key: keys){
//      System.out.println("Value of " + key + " is: " +
//      symbolTable.getAddress(key));
// }