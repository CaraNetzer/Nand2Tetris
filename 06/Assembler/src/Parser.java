import java.io.BufferedReader;
import java.io.FileReader;

public class Parser {
    public BufferedReader br;
    public String currentLine;

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

    public void advance() {
        try {
            this.currentLine = this.br.readLine();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public String commandType() {
        if(currentLine.charAt(0) == '@') {
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
            return currentLine.substring(1).trim();
        } else if (this.commandType() == "L_COMMAND") {
            return currentLine.substring(1, currentLine.length() - 1).trim();
        } else {
            return null;
        }
    }

    public String dest() {
        String result = "";
        try {
            String[] lineParts = currentLine.split("=");
            if(lineParts.length > 1) {
                result = lineParts[0].trim();
            } else if (lineParts.length == 1) {
                result = "null";
            }
            return result;
        } catch (ArrayIndexOutOfBoundsException e) {
            return null;
        }
    }

    public String comp() {
        try {
            String[] lineParts = currentLine.split("=");
            return lineParts[1].trim();
        } catch (ArrayIndexOutOfBoundsException e) {
            String[] lineParts = currentLine.split(";");
            return lineParts[0].trim();
        }
    }

    public String jump() {
        try {
            String[] lineParts = currentLine.split(";");
            return lineParts[1].trim();
        } catch (ArrayIndexOutOfBoundsException e) {
            return null;
        }
    }
}
