import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

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

    public boolean hasMoreCommands() {
      try {
        return br.readLine() != null;
      } catch (IOException e) {
        e.printStackTrace();
        return false;
      }
    }

    public void advance() {
      try {
        currentLine = br.readLine();
      } catch (IOException e) {
        e.printStackTrace();
      }
    }

    public String commandType() {
      if(currentLine == "add" || currentLine == "sub" || currentLine == "neg" || currentLine == "eq" ||
         currentLine == "gt"  || currentLine == "lt"  || currentLine == "and" || currentLine == "or" ||
         currentLine == "not") {
        return "C_ARITHMETIC";

      } else if(currentLine.substring(0,3) == "push") {
        return "C_PUSH";      
      } else if(currentLine.substring(0,2) == "pop") {
        return "C_POP";      
      } 
      /*else if() {
        return "C_LABEL";      
      } else if() {
        return "C_GOTO";      
      } else if() {
        return "C_IF";      
      } else if() {
        return "C_FUNCTION";      
      } else if() {
        return "C_CALL";      
      } */
      else {
        return "";
      }
    }

    public String arg1() {
      try{
        return currentLine.split(" ")[1];
      } catch(ArrayIndexOutOfBoundsException e) {
        return currentLine.split(" ")[0];
      }
    }
    
    public int arg2() {
      try{
        return Integer.parseInt(currentLine.split(" ")[2]);
      } catch(NumberFormatException e) {
        return -1;
      }
    }

}
