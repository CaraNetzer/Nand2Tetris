import java.util.HashMap;

public class Code {
    HashMap<String, String> destinationDictionary = new HashMap<String,String>();
    HashMap<String, String> jumpDictionary = new HashMap<String,String>();
    HashMap<String, String> computationDictionary = new HashMap<String,String>();

    public Code() {
        destinationDictionary.put("null", "000");
        destinationDictionary.put("M",    "001");
        destinationDictionary.put("D",    "010");
        destinationDictionary.put("MD",   "011");
        destinationDictionary.put("A",    "100");
        destinationDictionary.put("AM",   "101");
        destinationDictionary.put("AD",   "110");
        destinationDictionary.put("AMD",  "111");

        jumpDictionary.put("null", "000");
        jumpDictionary.put("JGT",  "001");
        jumpDictionary.put("JEQ",  "010");
        jumpDictionary.put("JGE",  "011");
        jumpDictionary.put("JLT",  "100");
        jumpDictionary.put("JNE",  "101");
        jumpDictionary.put("JLE",  "110");
        jumpDictionary.put("JMP",  "111");

        computationDictionary.put("0",   "0101010");
        computationDictionary.put("1",   "0111111");
        computationDictionary.put("-1",  "0111010");
        computationDictionary.put("D",   "0001100");
        computationDictionary.put("A",   "0110000");
        computationDictionary.put("!D",  "0001101");
        computationDictionary.put("!A",  "0110001");
        computationDictionary.put("-D",  "0001111");
        computationDictionary.put("-A",  "0110011");
        computationDictionary.put("D+1", "0011111");
        computationDictionary.put("A+1", "0110111");
        computationDictionary.put("D-1", "0001110");
        computationDictionary.put("A-1", "0110010");
        computationDictionary.put("D+A", "0000010");
        computationDictionary.put("D-A", "0010011");
        computationDictionary.put("A-D", "0000111");
        computationDictionary.put("D&A", "0000000");
        computationDictionary.put("D|A", "0010101");
        
        computationDictionary.put("M",   "1110000");
        computationDictionary.put("!M",  "1110001");
        computationDictionary.put("-M",  "1110011");
        computationDictionary.put("M+1", "1110111");
        computationDictionary.put("M-1", "1110010");
        computationDictionary.put("D+M", "1000010");
        computationDictionary.put("D-M", "1010011");
        computationDictionary.put("M-D", "1000111");
        computationDictionary.put("D&M", "1000000");
        computationDictionary.put("D|M", "1010101");
 
    }
    
    public String dest(String mnuemonic) {
        return destinationDictionary.get(mnuemonic);
    }

    public String jump(String mnuemonic) {
        return jumpDictionary.get(mnuemonic);
    }
    
    public String comp(String mnuemonic) {
        return computationDictionary.get(mnuemonic);
    }
}
