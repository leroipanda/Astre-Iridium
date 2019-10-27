import javax.mail.*;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMultipart;
import java.io.IOException;
import java.util.Properties;

public class ReadEmail {

    Properties props;

    public ReadEmail(String username){
        props = new Properties();
        props.put("mail.pop3.socketFactory.class", "javax.net.ssl.SSLSocketFactory");
        props.put("mail.pop3.socketFactory.fallback", "false");
        props.put("mail.pop3.socketFactory.port", "995");
        props.put("mail.pop3.port", "995");
        props.put("mail.pop3.host", "pop.gmail.com");
        props.put("mail.pop3.user", username);
        props.put("mail.store.protocol", "imaps");
    }

    public void read(String username,String mdp){

        // 2. Creates a javax.mail.Authenticator object.
        Authenticator auth = new Authenticator() {
            @Override
            protected PasswordAuthentication getPasswordAuthentication() {
                return new PasswordAuthentication(username, mdp);
            }
        };

        // 3. Creating mail session.
        Session session = Session.getDefaultInstance(props, auth);

        try {
            // 4. Get the POP3 store provider and connect to the store.
            Store store = session.getStore("imaps");
            store.connect("pop.gmail.com", "louis.blenner@gmail.com", mdp);

            /* Affiche les folder dispo sur la boite
            Folder[] f = store.getFolder("[Gmail]").list();
            for(Folder fd:f)
                System.out.println(">> "+fd.getName());
            */

            // 5. Get folder and open the INBOX folder in the store.
            Folder inbox = store.getFolder("[Gmail]/Tous les messages");
            inbox.open(Folder.READ_ONLY);

            // 6. Retrieve the messages from the folder.
            Message[] messages = inbox.getMessages();

            int i = messages.length-1;
            boolean msgVu = false;
            while (i >= 0 && !msgVu){
                //recuperation de l'expediteur
                Address[] froms = messages[i].getFrom();
                String emailaddr = froms == null ? null : ((InternetAddress) froms[0]).getAddress();

                if (emailaddr.equals("louis.blenner@gmail.com")){
                    Message message = messages[i];
                    System.out.println(getTextFromMessage(message));
                    msgVu = true;
                }
                i--;
            }
            if (msgVu = false){
                System.out.println("Expediteur non trouvé");
            }


            // 7. Close folder and close store.
            inbox.close(false);
            store.close();

        } catch(MessagingException e) {
            e.printStackTrace();
        } catch(Exception e) {
            e.printStackTrace();
        }
    }

    private String getTextFromMessage(Message message) throws MessagingException, IOException {
        String result = "";
        if (message.isMimeType("text/plain")) {
            result = message.getContent().toString();
        } else if (message.isMimeType("multipart/*")) {
            MimeMultipart mimeMultipart = (MimeMultipart) message.getContent();
            result = getTextFromMimeMultipart(mimeMultipart);
        }
        return result;
    }

    private String getTextFromMimeMultipart(
            MimeMultipart mimeMultipart)  throws MessagingException, IOException {
        String result = "";
        int count = mimeMultipart.getCount();
        for (int i = 0; i < count; i++) {
            BodyPart bodyPart = mimeMultipart.getBodyPart(i);
            if (bodyPart.isMimeType("text/plain")) {
                result = result + "\n" + bodyPart.getContent();
                break; // without break same text appears twice in my tests
            } else if (bodyPart.getContent() instanceof MimeMultipart) {
                result = result + getTextFromMimeMultipart((MimeMultipart) bodyPart.getContent());
            }
        }
        return result;
    }
}

