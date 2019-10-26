import javax.mail.*;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;
import java.util.Properties;
import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
        //final String username = "louis.blenner";
        //final String password = ""; //Mon mdp d'application Pas touche

        MainProcess go = new MainProcess();
        go.routine();
    }

}
