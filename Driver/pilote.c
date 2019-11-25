#include<stdio.h> 
#include<stdlib.h>

//definir taille message retour du modeme
//finir la machine à état qq oublie 
//regarder pour ip uart pour les com vers carte 
//modifier la fonction enfiler pour ajouter un bool en sortie qui indique si le message est bien ajouter au buffer
//réflechire au pipe à lire 

//----------------------------------------FAIRE LE MODULE FIFO---------------------------------------------------------
#define tailleBuffer 6
#define tailleMessageBuffer 5 


char bufferValeur [tailleBuffer] ;
char *buffer   = bufferValeur ; 

typedef struct Element Element;
struct Element
{
    char texte[tailleMessageBuffer];
    int nbElements  ; 
};

typedef struct Buffer Buffer ; 
struct Buffer
{
    Element buf[tailleBuffer];
    int premier ;
    int posiBuffer ; 
    int nbElement ;  //pas implementer 
};

 


void initialiser(Buffer *buffer ){
    buffer->premier =0  ;
    buffer->posiBuffer=0 ;
    buffer->nbElement = 0 ; 
    /* on va s'assurer que toute les valeur sont bien initialise */ 
    for(int i = 0 ; i< tailleBuffer ; i++ ){
        for(int j = 0 ; j < tailleMessageBuffer ; j++ ){
            buffer->buf[i].texte[j] = 'a' ; 
        }
        buffer->buf[i].nbElements = 0 ;
    }
}

void enfiler(Buffer *buffer, char message[tailleMessageBuffer],int tailleTexte)
{
    if(buffer->nbElement < tailleBuffer ){
    
    int nb = buffer->nbElement ; 
    buffer->nbElement =  nb +1  ;

    for(int i=0; i<tailleTexte; i++){
       /* e.texte[i] = message[i] ; */
        buffer->buf[buffer->posiBuffer].texte[i] = message[i] ;
    } 
        buffer->buf[buffer->posiBuffer].nbElements = tailleTexte;
    
     if(buffer->posiBuffer == tailleBuffer -1 ){
        buffer->posiBuffer = 0 ;
        printf("%s" ,"ok");  
   }
   else {
       buffer->posiBuffer = (buffer->posiBuffer) +1  ;
   } 

    }
    else{
        //le buffer est plein !!!
    }

} 

struct Element defiler(Buffer *buffer)
{
    if (buffer == NULL)
    {
        printf("exit") ; 
        exit(-1);
    }
    
    int posi = buffer->premier  ;
    int nb = buffer->nbElement ;
    if(buffer->nbElement == 0 ){exit(-1);} // /!\ si buffer vide => exit !!!!!
    buffer->nbElement = nb -1 ;
    buffer->premier = posi +1 ;  
    return buffer->buf[posi] ; 
    
}

int nbElementBuffer(Buffer *buffer){
     return buffer->nbElement ;
}



//---------------------------------------------FIN MODULE FIFO---------------------------------------------------------



struct Buffer vbufferODBH  ;
struct Buffer vbufferModem ;
struct Buffer *bufferODBH = &vbufferODBH ;
struct Buffer *bufferModem =  &vbufferModem;



enum Etats
{
    SET_UP = 1,
    READ_STATUS = 2,
    READ_MESG =3,
    PREPARE_WRITE=4 ,
    WRITE_MESG=5,
    CLEAN_WRITE_BUFFER=6,
    INIT_SESSION =7,
    CHECK_ALIVE =8
    };


enum Etats etatActuel = SET_UP; 
enum bool isAlive ;  // variable suprimable
enum bool statut[2] ; 
char *msg ;
int send_msg_count = 0 ;


char commandeModem [tailleMessageBuffer] ;
char reponseModem  [tailleMessageBuffer];


void messageModem( ){

} 
void messageModemAvecReponse(char *reponse  ){
    //modifier la valeur de réponse avec le retour du modem
}


//fonction qui ecrit une commande simple dans le bufffer 

void ecritureCmdBuffer(char cmd[] ,int tailleTexte ){
    printf("écritureCmdBuffer\n" ) ;
    enfiler(bufferModem ,cmd , tailleTexte ) ;  
    

}

//fonction qui envoie une commande simple au modem  
void nouvelleCommande(char cmd[] ,int tailleTexte ){
    printf("nouvelleComande !!\n") ;
    ecritureCmdBuffer( cmd ,tailleTexte);

    // /!\ il faut veiller à ce que la partie iuart s'ammuse avec le buffer
}

//fonction qui envoie une commande avec un retour au modem
void nouvelleCommandeAvecRetour(char cmd[] ,int tailleTexe ,char reponse []  ){
    printf("nouvelleCommandeAvecRetour\n") ;
    ecritureCmdBuffer( cmd ,tailleTexe);
    messageModemAvecReponse( reponse) ;
     
    
    
}

void main(){

printf("je commence \n") ; 
initialiser( bufferModem) ; 
initialiser(bufferODBH) ;
etatActuel = SET_UP ;



while (1)
{
    //printf("je rentre dans la boucle while \n ") ; 

    switch (etatActuel)//je regarde dans quel etat je suis 
    {
    case 1:
        printf("set up\n") ; 
        nouvelleCommande("ATE0", 5) ;
        nouvelleCommande("AT+SDBD2", 9) ;
        nouvelleCommande("AT+SBDC" ,8) ;
        etatActuel = READ_STATUS ;
        break;
    case READ_STATUS:
        nouvelleCommandeAvecRetour("AT+SBDS",8,reponseModem) ;
        if(  reponseModem[2] =='0' ) {
           etatActuel=  PREPARE_WRITE ;
        }
        else{
          etatActuel=  READ_MESG ;  
        }
        break;

    case READ_MESG:
        nouvelleCommandeAvecRetour("AT+SBDRB",9 ,msg);
        // ------ faire la file -------//
        nouvelleCommande("AT+SBDD1",9);
        etatActuel = PREPARE_WRITE ;
        break;
    
    case PREPARE_WRITE:
 //TODO:fixeme



        
        break;
    
    
    case CLEAN_WRITE_BUFFER:

        
        break;
    case INIT_SESSION: 

        nouvelleCommande("SBD1",5) ;
        etatActuel = CHECK_ALIVE ;
        break;
    case CHECK_ALIVE:

        nouvelleCommandeAvecRetour("AT" ,3,reponseModem );
        if (reponseModem[0] == '1' ){
            etatActuel = READ_STATUS ;
        }
        else{
            etatActuel = INIT_SESSION ;
        }
        

        break;

    
    default:
        break;
    }
    
} 
}