import javax.mail.*;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;
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

            Session session = Session.getInstance(prop,
                new javax.mail.Authenticator() {
                    protected PasswordAuthentication getPasswordAuthentication() {
                        return new PasswordAuthentication(username, mdp);
                    }
                });

            try {
                System.out.print("Adresse destination : ");
                dest = scan.next();
                Message message = new MimeMessage(session);
                message.setFrom(new InternetAddress(username));
                message.setRecipients(
                    Message.RecipientType.TO,
                    InternetAddress.parse("louis.blenner@gmail.com")
                );
                System.out.print("Sujet : ");
                String suj = scan.next();
                System.out.print("Message : ");
                String msg = scan.next();
                message.setSubject(suj);
                message.setText(msg);

                Transport.send(message);

                System.out.println("Succés");

            } catch (AuthenticationFailedException e) {
                System.out.println("Authentification échoué");
            } catch (MessagingException e) {
                e.printStackTrace();
            }
        }
    }
}
