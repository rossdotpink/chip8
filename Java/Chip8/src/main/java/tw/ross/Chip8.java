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
            for(int i = 0; i < 4096; i+=2) {
                System.out.printf("%02x%02x\n", romBytes[i], romBytes[i+1]);
            }
        } catch (Exception c) {
            System.out.println("Uh oh");
        }
    }

}
