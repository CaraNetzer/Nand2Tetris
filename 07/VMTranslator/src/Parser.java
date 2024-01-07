import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class Parser {
    public BufferedReader br;
    public String currentLine = "";

    public Parser(String filePath) {
      System.out.println(filePath + " parser");  
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
        System.out.println(currentLine);  
      } catch (IOException e) {
        e.printStackTrace();
      }
    }

    public String commandType() {
      String command = currentLine.trim().split(" ")[0];

      if(command.contains("add") || command.contains("sub") || command.contains("neg") || 
         command.contains("eq")  || command.contains("gt")  || command.contains("lt")  || 
         command.contains("and") || command.contains("or")  || command.contains("not")) {
        System.out.println("cmd type: arithmetic");  
        return "C_ARITHMETIC";
           
      } else if(command.contains("push")) {
        System.out.println("cmd type: push"); 
        return "C_PUSH";      
        
      } else if(command.contains("pop")) {
        System.out.println("cmd type: pop"); 
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
