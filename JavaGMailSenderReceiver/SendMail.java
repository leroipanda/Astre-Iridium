import javax.mail.*;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;
import java.util.Properties;

public class SendMail {

    Properties prop;
    public SendMail(){
        prop = new Properties();
        prop.put("mail.smtp.host", "smtp.gmail.com");
        prop.put("mail.smtp.port", "587");
        prop.put("mail.smtp.auth", "true");
        prop.put("mail.smtp.starttls.enable", "true"); //TLS
    }

    public void send(String username, String mdp,String dest,String sujet,String msg){

        Session session = Session.getInstance(prop,
                new javax.mail.Authenticator() {
                    protected PasswordAuthentication getPasswordAuthentication() {
                        return new PasswordAuthentication(username, mdp);
                    }
                });

        try {
            Message message = new MimeMessage(session);
            message.setFrom(new InternetAddress(username));
            message.setRecipients(
                    Message.RecipientType.TO,
                    InternetAddress.parse(dest)
            );
            message.setSubject(sujet);
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
