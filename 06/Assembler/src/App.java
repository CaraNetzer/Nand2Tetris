import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;

//to run in command line:
//javac App.java
//java App ../../add/Add.asm
public class App {
    public static void main(String[] args) throws Exception {

        String filePath = args[0];
        try {
            String[] inFileNameArray = filePath.split("/");
            String inFileName = inFileNameArray[inFileNameArray.length - 1];
            String outFileName = inFileName.substring(0, inFileName.length() - 3) + "hack";

            File file = new File(outFileName);
            file.createNewFile();

            int currentLine = 0;

            try (BufferedReader br = new BufferedReader(new FileReader(filePath));) {
                try (BufferedWriter bw = new BufferedWriter(new FileWriter(outFileName));) {
                    for (String line; (line = br.readLine()) != null; currentLine++) {
                        if (!line.trim().isEmpty() && line.charAt(0) != '/') {
                            System.out.println(line);

                            // process a-instructions
                            if (line.charAt(0) == '@') {
                                int number = Integer.parseInt(line.substring(1));
                                String intBaseTwo = String.format("%16s", Integer.toBinaryString(number)).replace(' ',
                                        '0');
                                bw.write(intBaseTwo + "\n");
                            }
                            // process c-instructions
                            else {

                            }
                        }
                    }
                }
            }

        } catch (Exception e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
    }
}