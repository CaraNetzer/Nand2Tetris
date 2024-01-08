import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class Parser {
    public BufferedReader br;
    public String currentLine = "";

    public Parser(String filePath) {
        try {
            br = new BufferedReader(new FileReader(filePath));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public boolean hasMoreCommands() {
      return currentLine != null;      
    }

    public void advance() {
      try {
        currentLine = br.readLine();        
        //System.out.println("current line: " + currentLine);
      } catch (IOException e) {
        e.printStackTrace();
      }
    }

    public String commandType() {
      String command = currentLine.trim().split(" ")[0];

      if(command.equals("add") || command.equals("sub") || command.equals("neg") || 
         command.equals("eq")  || command.equals("gt")  || command.equals("lt")  || 
         command.equals("and") || command.equals("or")  || command.equals("not")) {
        return "C_ARITHMETIC";
           
      } else if(command.equals("push")) {
        return "C_PUSH";      
        
      } else if(command.equals("pop")) {
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
        return currentLine.trim().split(" ")[1];
      } catch(ArrayIndexOutOfBoundsException e) {
        return currentLine.trim().split(" ")[0];
      }
    }
    
    public int arg2() {
      try{
        return Integer.parseInt(currentLine.trim().split(" ")[2]);
      } catch(NumberFormatException e) {
        return -1;
      }
    }

}
