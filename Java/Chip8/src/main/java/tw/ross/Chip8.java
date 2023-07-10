package tw.ross;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Path;
/**
 * Hello world!
 *
 */
public class Chip8 
{
    public static void main( String[] args )
    {
        loadFile();
        System.out.println( "Hello World!" );
    }

    public static void loadFile() {
        try {
            InputStream romFileStream = Files.newInputStream(Path.of("./test.ch8"));
            byte[] romBytes = romFileStream.readNBytes(4096);
            for(int i = 0; i < romBytes.length; i+=2) {
                String opCode = String.format("%02X%02X", romBytes[i], romBytes[i+1]);
                System.out.println(opCode);
                switch (opCode) {
                    case "00E0": System.out.println("Clear Screen!");
                                break;
                    default: break;
                }
            }
        } catch (Exception c) {
            System.out.println("Uh oh");
        }
    }

}
