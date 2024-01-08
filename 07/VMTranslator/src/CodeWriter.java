import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;

public class CodeWriter {
    public BufferedWriter bw;
    public String currentLine;
    public String filename;
    public int labelCounter = 0;

    public HashMap<String, String> segmentPointers = new HashMap<String, String>() {
        {
            put("local", "LCL"); 
            put("argument", "ARG"); 
            put("this", "THIS"); 
            put("that", "THAT"); 
        }
    };

    public HashMap<String, String> math = new HashMap<String, String>() {
        {
            put("add", "+"); 
            put("sub", "-"); 
            put("and", "&"); 
            put("or", "|"); 
            put("neg", "-"); 
            put("not", "!"); 
        }
    };
        
    public CodeWriter(String filePath) throws IOException {    
        System.out.println(filePath);    
        bw = new BufferedWriter(new FileWriter("../outFiles/" + filePath));   
        filename = filePath.substring(0, filePath.indexOf("."));   
    }

    public void writeArithmetic(String command) throws IOException {
        StringBuilder sb = new StringBuilder();
        sb.append("//" + command + "\n");
        switch (command) {
            case "add":
            case "sub":
            case "and":
            case "or":
                saveArgsAndCompute(sb, math.get(command));                
                pushBackOntoStack(sb);               
                break;

            case "neg":
            case "not":
                //SP--
                sb.append("@SP\n");
                sb.append("M=M-1\n");

                //D=(-/!)RAM[SP]
                sb.append("A=M\n");
                sb.append("D=" + math.get(command) + "M\n");

                pushBackOntoStack(sb);

                break;

            case "eq":
            case "lt":
            case "gt":
                saveArgsAndComputeBool(sb, command);
                pushBackOntoStack(sb);                

                break;                
            
            default:
                break;
        }
        bw.write(sb.toString());
    }

    public void saveArgsAndCompute(StringBuilder sb, String computation) {
        //SP--
        sb.append("@SP\n");
        sb.append("M=M-1\n");

        //D=RAM[SP]
        sb.append("A=M\n");
        sb.append("D=M\n");

        //put first arg in R13
        sb.append("@R13\n"); 
        sb.append("M=D\n");  
        
        //repeat that
        //SP--
        sb.append("@SP\n");
        sb.append("M=M-1\n");                
        //D=RAM[SP]
        sb.append("A=M\n");
        sb.append("D=M\n");                
        //put second arg in R14
        sb.append("@R14\n"); 
        sb.append("M=D\n");
        //sb.append("D=M\n");

        // +, -, &, or |
        sb.append("@R13\n"); 
        sb.append("D=D" + computation + "M\n");
    }

    public void saveArgsAndComputeBool(StringBuilder sb, String bool) {
        //SP--
        sb.append("@SP\n");
        sb.append("M=M-1\n");

        //D=RAM[SP]
        sb.append("A=M\n");
        sb.append("D=M\n");

        //put first arg in R13
        sb.append("@R13\n"); 
        sb.append("M=D\n");  
        
        //repeat that
        //SP--
        sb.append("@SP\n");
        sb.append("M=M-1\n");                
        //D=RAM[SP]
        sb.append("A=M\n");
        sb.append("D=M\n");                
        //put second arg in R14
        sb.append("@R14\n"); 
        sb.append("M=D\n");
        //sb.append("D=M\n");

        // D = first number
        sb.append("@R14\n");
        sb.append("D=M\n");  

        // D = x - y
        sb.append("@R13\n"); 
        sb.append("D=D-M \n"); 

        // if D (eq/gt/lt) 0 (bool) goto TRUE
        sb.append("@TRUE." + labelCounter + "\n"); 
        sb.append("D;J" + bool.toUpperCase() + "\n");

        // else goto FALSE
        sb.append("@FALSE." + labelCounter + "\n"); 
        sb.append("0;JMP\n");
        
        sb.append("(TRUE." + labelCounter + ")\n"); 
        sb.append("D=-1\n");
        sb.append("@END." + labelCounter + "\n"); 
        sb.append("0;JMP\n");
        
        sb.append("(FALSE." + labelCounter + ")\n"); 
        sb.append("D=0\n"); 

        sb.append("(END." + labelCounter + ")\n"); 

        labelCounter++;
    }

    public void pushBackOntoStack(StringBuilder sb) {
        //push that back onto the stack
        //RAM[SP] = D
        sb.append("@SP\n");
        sb.append("A=M\n");
        sb.append("M=D\n");

        //SP++
        sb.append("@SP\n");
        sb.append("M=M+1\n");

    }

    //TO-DO: optimize write-push-pop
    public void writePushPop(String command, String segment, int index) throws IOException {
        System.out.println(command + "writepushpop");  
        
        StringBuilder sb = new StringBuilder();

        if (segment == "local" || segment == "argument" || segment == "this" || segment == "that") {
            if(command == "C_PUSH") {
                
                sb.append("//push " + segment + " " + index + "\n");
                
                //addr = segmentPointers.get(segment) + index
                sb.append("@" + segmentPointers.get(segment) + "\n");
                sb.append("D=M\n");
                sb.append("@" + index + "\n");
                sb.append("D=D+A\n");
                
                //RAM[SP] = RAM[addr]
                sb.append("A=D\n"); //M of this will be addr
                sb.append("D=M\n"); //RAM[addr]

                sb.append("@SP\n");
                sb.append("A=M\n");
                sb.append("M=D\n");

                //SP++
                sb.append("@SP\n");
                sb.append("M=M+1\n");

            } else if (command == "C_POP") {
                sb.append("//pop " + segment + " " + index + "\n");
                
                //addr = segmentPointers.get(segment) + index
                sb.append("@" + segmentPointers.get(segment) + "\n");
                sb.append("D=M\n");
                sb.append("@" + index + "\n");
                sb.append("D=D+A\n");
                sb.append("@R13\n"); 
                sb.append("M=D\n"); //store address in R13
                
                //SP--
                sb.append("@SP\n");
                sb.append("M=M-1\n");
                
                //RAM[addr] = RAM[SP]         
                sb.append("@SP\n"); //do i need this line?
                sb.append("A=M\n");
                sb.append("D=M\n");

                sb.append("@R13\n"); 
                sb.append("A=M\n");
                sb.append("M=D\n");                
            }
        }
        
        switch (segment) {    
            case "constant" :
                sb.append("//push constant " + index + "\n");

                //D=index
                sb.append("@" + index + "\n");
                sb.append("D=A\n");
                
                //RAM[SP]=D
                sb.append("@SP\n");
                sb.append("A=M\n");
                sb.append("M=D\n");
                
                //SP++
                sb.append("@SP\n");
                sb.append("M=M+1\n");


                break;
            case "static":
                if(command == "C_PUSH") {
                    sb.append("//push " + segment + " " + index + "\n");
                    
                    //RAM[SP] = RAM[addr]
                    sb.append("@" + filename + "." + index + "\n");
                    sb.append("D=M\n"); //RAM[addr]
                    
                    sb.append("@SP\n");
                    sb.append("A=M\n");
                    sb.append("M=D\n");

                    //SP++
                    sb.append("@SP\n");
                    sb.append("M=M+1\n");                    

                } else if (command == "C_POP") {
                    sb.append("//pop " + segment + " " + index + "\n");
                    
                    //SP--
                    sb.append("@SP\n");
                    sb.append("M=M-1\n");
                    
                    //RAM[addr] = RAM[SP]         
                    sb.append("@SP\n"); //do i need this line?
                    sb.append("A=M\n");
                    sb.append("D=M\n");
                    
                    sb.append("@" + filename + "." + index + "\n");
                    sb.append("M=D\n");                
                }
                break;            
            case "temp":
                if(command == "C_PUSH") {
                
                    sb.append("//push " + segment + " " + index + "\n");
                    
                    //addr = 5 + index
                    sb.append("@5\n");
                    sb.append("D=A\n");
                    sb.append("@" + index + "\n");
                    sb.append("D=D+A\n");
                    
                    //RAM[SP] = RAM[addr]
                    sb.append("A=D\n"); 
                    sb.append("D=M\n"); //RAM[addr]

                    sb.append("@SP\n");
                    sb.append("A=M\n");
                    sb.append("M=D\n");

                    //SP++
                    sb.append("@SP\n");
                    sb.append("M=M+1\n");

                } else if (command == "C_POP") {
                    sb.append("//pop " + segment + " " + index + "\n");
                    
                    //addr = 5 + index
                    sb.append("@5\n");
                    sb.append("D=A\n");
                    sb.append("@" + index + "\n");
                    sb.append("D=D+A\n");
                    sb.append("@R14\n"); 
                    sb.append("M=D\n"); //store address in R14
                    
                    //SP--
                    sb.append("@SP\n");
                    sb.append("M=M-1\n");
                    
                    //RAM[addr] = RAM[SP]         
                    sb.append("@SP\n"); //do i need this line?
                    sb.append("A=M\n");
                    sb.append("D=M\n");

                    sb.append("@R14\n"); 
                    sb.append("A=M\n");
                    sb.append("M=D\n");                
                }
                break;        
            case "pointer":
                if(command == "C_PUSH") {
                
                    sb.append("//push " + segment + " " + index + "\n");
                    
                    //addr = 3 + index
                    sb.append("@3\n");
                    sb.append("D=A\n");
                    sb.append("@" + index + "\n");
                    sb.append("D=D+A\n");
                    
                    //RAM[SP] = RAM[addr]
                    sb.append("A=D\n"); 
                    sb.append("D=M\n"); //RAM[addr]

                    sb.append("@SP\n");
                    sb.append("A=M\n");
                    sb.append("M=D\n");

                    //SP++
                    sb.append("@SP\n");
                    sb.append("M=M+1\n");

                } else if (command == "C_POP") {
                    sb.append("//pop " + segment + " " + index + "\n");
                    
                    //addr = 3 + index
                    sb.append("@3\n");
                    sb.append("D=A\n");
                    sb.append("@" + index + "\n");
                    sb.append("D=D+A\n");
                    sb.append("@R15\n"); 
                    sb.append("M=D\n"); //store address in R15
                    
                    //SP--
                    sb.append("@SP\n");
                    sb.append("M=M-1\n");
                    
                    //RAM[addr] = RAM[SP]         
                    sb.append("@SP\n"); //do i need this line?
                    sb.append("A=M\n");
                    sb.append("D=M\n");

                    sb.append("@R15\n"); 
                    sb.append("A=M\n");
                    sb.append("M=D\n");           
                }
                break;
            default:
                break;
        }

        bw.write(sb.toString());
    }

    public void close() {
      try {
        bw.close();
      } catch (IOException e) {
        e.printStackTrace();
      }
    }
}
