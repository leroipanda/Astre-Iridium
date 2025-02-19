import javax.mail.*;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;
import java.io.IOException;
import java.util.Properties;
import java.util.Scanner;

public class MainProcess {

    private String username;
    private String mdp;
    private String dest;
    Scanner scan = new Scanner(System.in);

    public MainProcess(){

    }

    public void routine() {
        Properties prop = new Properties();
        prop.put("mail.smtp.host", "smtp.gmail.com");
        prop.put("mail.smtp.port", "587");
        prop.put("mail.smtp.auth", "true");
        prop.put("mail.smtp.starttls.enable", "true"); //TLS

        while(true) {
            System.out.print("Entrez votre adresse gmail: ");
            username = scan.next();
            System.out.print("Entrez votre mot de passe application : ");
            mdp = scan.next();

            ReadEmail reader = new ReadEmail(username);

            reader.read(username,mdp);

            System.out.print("Adresse destination : ");
            dest = scan.next();
            System.out.print("Sujet : ");
            String suj = scan.next();
            System.out.print("Message : ");
            String msg = scan.next();

            SendMail sender = new SendMail();
            sender.send(username,mdp,dest,suj,msg);

            System.out.println("Succés");

        }
    }
}
