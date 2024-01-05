import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

public class CodeWriter {
    public BufferedWriter bw;
    public String currentLine;

    public CodeWriter(String filePath) {
        try {
            bw = new BufferedWriter(new FileWriter("../outFiles/" + filePath));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void writeArithmetic(String command) {
      switch (command) {
        case "add":
            break;
        case "sub":
            break;
        case "neg":
            break;
        case "eq":
            break;
        case "lt":
            break;
        case "gt":
            break;
        case "and":
            break;
        case "or":
            break;
        case "not":
            break;
        default:
            break;
      }
    }

    public void writePushPop(String command, String segment, int index) {
        switch (segment) {
            case "arguement":
                
                break;
            case "local":
                
                break;
            case "static":
                
                break;
            case "constant":
                
                break;
            case "this":
                
                break;
            case "that":
                
                break;
            case "pointer":
                
                break;
            case "temp":
                
                break;
        
            default:
                break;
        }
    }

    public void close() {
      try {
        bw.close();
      } catch (IOException e) {
        e.printStackTrace();
      }
    }
}
