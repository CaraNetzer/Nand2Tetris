import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;

public class CodeWriter {
    public BufferedWriter bw;
    public String currentLine;

    public HashMap<String, String> segmentPointers = new HashMap<String, String>() {
        {
            put("local", "LCL"); 
            put("arguement", "ARG"); 
            put("this", "THIS"); 
            put("that", "THAT"); 
        }
    };
        
    public CodeWriter(String filePath) throws IOException {        
        bw = new BufferedWriter(new FileWriter("../outFiles/" + filePath));        
    }

    public void writeArithmetic(String command) throws IOException {
        bw.write("//" + command + "\n");
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

    public void writePushPop(String command, String segment, int index) throws IOException {
        if (segment == "local" || segment == "arguement" || segment == "this" || segment == "that") {
            if(command == "C_PUSH") {
                bw.write("//push " + segment + " " + index + "\n");
                
                //addr = segmentPointers.get(segment) + index
                bw.write("@" + segmentPointers.get(segment));
                bw.write("D=M\n");
                bw.write("@" + index + "\n");
                bw.write("D=D+A\n");
                
                //RAM[SP] = RAM[addr]
                bw.write("A=D\n"); //M of this will be addr
                bw.write("D=M\n"); //RAM[addr]

                bw.write("@SP\n");
                bw.write("A=M\n");
                bw.write("M=D\n");

                //SP++
                bw.write("@SP\n");
                bw.write("M=M+1\n");

            } else if (command == "C_POP") {
                bw.write("//pop " + segment + " " + index + "\n");
                
                //addr = segmentPointers.get(segment) + index
                bw.write("@" + segmentPointers.get(segment));
                bw.write("D=M\n");
                bw.write("@" + index + "\n");
                bw.write("D=D+A\n");
                bw.write("@R13\n"); 
                bw.write("M=D\n"); //store address in R13
                
                //SP--
                bw.write("@SP\n");
                bw.write("M=M-1\n");
                
                //RAM[addr] = RAM[SP]         
                bw.write("@SP\n"); //do i need this line?
                bw.write("A=M\n");
                bw.write("D=M\n");

                bw.write("@R13\n"); 
                bw.write("A=M\n");
                bw.write("M=D\n");                
            }
        }
        
        switch (segment) {    
            case "constant" :
                bw.write("//push constant " + index + "\n");

                //D=index
                bw.write("@" + index + "\n");
                bw.write("D=A\n");
                
                //RAM[SP]=D
                bw.write("@SP\n");
                bw.write("A=M\n");
                bw.write("M=D\n");
                
                //SP++
                bw.write("@SP\n");
                bw.write("M=M+1\n");


                break;
            case "static":
                
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
