import java.io.BufferedReader;
import java.io.FileReader;

public class Parser {
    public static BufferedReader br;
    public static String currentLine;

    public Parser(String filePath) {
        try {
            br = new BufferedReader(new FileReader(filePath));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public Boolean hasMoreCommands() {
        try {
            return br.readLine() != null;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    public static void advance() {
        try {
            currentLine = br.readLine();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public String commandType() {
        if(currentLine.substring(1,2) == "@") {
            return "A_COMMAND";
        } else if (currentLine.contains("=") || currentLine.contains(";")) {
            return "C_COMMAND";
        } else if (currentLine.contains("(")) {
            return "L_COMMAND";
        } else {
            return null;
        }
    }

    public String symbol() {
        if (this.commandType() == "A_COMMAND") {
            return currentLine.substring(1);
        } else if (this.commandType() == "L_COMMAND") {
            return currentLine.substring(1, currentLine.length() - 1);
        } else {
            return null;
        }
    }

    public String dest() {
        try {
            String[] lineParts = currentLine.split("=");
            return lineParts[0];
        } catch (ArrayIndexOutOfBoundsException e) {
            return null;
        }
    }

    public String comp() {
        try {
            String[] lineParts = currentLine.split("=");
            return lineParts[1];
        } catch (ArrayIndexOutOfBoundsException e) {
            return null;
        }
    }

    public String jump() {
        try {
            String[] lineParts = currentLine.split(";");
            return lineParts[1];
        } catch (ArrayIndexOutOfBoundsException e) {
            return null;
        }
    }
}
