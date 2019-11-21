import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Scanner;

import org.apache.commons.io.FileUtils;
import org.apache.commons.text.StringEscapeUtils;

public class Test {
	
	public static void main(String[] args) throws IOException {
		Scanner scan = new Scanner(System.in);
		System.out.print("Attachment (file path) : ");
        String atcm = scan.next();
        String file = atcm;
        String[] file_strs = file.split("\\\\");
        String fileName = file_strs[file_strs.length -1];
        System.out.println(fileName);
	}

}
