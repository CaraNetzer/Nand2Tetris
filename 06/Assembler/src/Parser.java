import java.io.BufferedReader;
import java.io.FileReader;

public class Parser {
    BufferedReader br;

    public Parser(String filePath) {
        try {
            br = new BufferedReader(new FileReader(filePath));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public Boolean hasMoreCommands() {
        try {
            if (br.readLine() != null) {
                return true;
            }
            return false;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    // public static advance() {
        
    // }
}
