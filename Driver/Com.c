#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "file.h"

typedef enum Etat Etat;
enum Etat
{
    SETUP,
    READ_STATUS,
    PREPARE_WRITE,
    CLEAN_WRITE_BUFFER,
    WRITE_MESG,
    READ_MESG,
    INIT_SESSION,
    CHECK_ALIVE
};

Etat etat = SETUP;
int i = 1;
int status[2];
int msg[7];
File *fifo_recv;
File *fifo_send;
int sent_mesg_count = 0;

void printEtat()
{
    switch (etat)
    {

    case SETUP:
        printf("%s","SETUP\n");
        break;

    case READ_STATUS:
        printf("%s","READ_STATUS\n");
        break;

    case READ_MESG:
        printf("%s","READ_MESG\n");
        break;

    case PREPARE_WRITE:
        printf("%s","PREPARE_WRITE\n");
        break;

    case CLEAN_WRITE_BUFFER:
        printf("%s","CLEAN_WRITE_BUFFER\n");
        break;

    case WRITE_MESG:
        printf("%s","WRITE_MESG\n");
        break;

    case INIT_SESSION:
        printf("%s","INIT_SESSION\n");
        break;

    case CHECK_ALIVE:
        printf("%s","CHECK_ALIVE\n");
        break;
    }
}

void ATE0() //Echo commande (0 = false, 1 = true)
{
}

void ATSBDD2() 
/*clear mobile originated (MO) and terminated buffer (MT)
2 = both, 1 = terminated, 0 = originated
return 0 = success, 1 = error
*/
{
    
}
void ATSBDD1()
{
}

void ATSBDD0()
{
}

void ATSBDC()
/*Clear the mobile originated message seqeunce number (MOMS)
0 = success, 1 = error*/
{
}

void ATSBDS(int *status,int i)
/*Return state
[MO,MOMSN,MT,MTMSN]
Mo : 0 = no message, 1 = message in buffer
MOMSN : sequence number that will be used in the next mobile originated SBD
MT : 0 = no message, 1 = message in buffer
MTMSN : sequence nuber used in last MT SBD session (-1 par defaut)*/
{
    status[0] = i;
    status[1] = i;
    status[2] = i;
    status[3] = i;
}

void ATSBDRB(int *msg)
/*This command is used to transfer a binary SBD message from the single mobile terminated buffer in the
9602 to the DTE. The mobile terminated buffer can contain only one message at any one time.
format {2-byte message length} + {binary SBD message} + {2-byte checksum}

The {2-byte message length} field represents the length, in bytes, of the SBD message not
including the length field or the mandatory two-byte checksum. The high order byte will be sent
first.
The maximum mobile terminated SBD message length is length is specified at 270 bytes (and
may be increased following actual performance testing).
The checksum is the least significant 2-bytes of the summation of the entire SBD message. The
high order byte will be sent first. For example if the 9602 were to send the word “hello” encoded in
ASCII to the DTE the binary stream would be hex 00 05 68 65 6c 6c 6f 02 14.
If there is no mobile terminated SBD message waiting to be retrieved from the 9602, the message
length and checksum fields will be zero.
The mobile terminated message buffer will be empty upon power-up.*/
{
    for(i=0;i<8;i++){
        msg[i] = i;
    }
    
}



void ATSBDWB()
/*Envoie ou recois message depuis/dans les buffer vers/depuis le DTE

This command is used to transfer a binary SBD message from the DTE to the single mobile originated
buffer in the 9602. The mobile originated buffer can contain only one message at any one time.
Once the command is entered, and the message length is acceptable, the 9602 will indicate to the
DTE that it is prepared to receive the message by sending the ASCII encoded string
“READY<CR><LF>” (hex 52 45 41 44 59 0D 0A) to the DTE.
The <SBD message length> parameter represents the length, in bytes, of the SBD message not
including the mandatory two-byte checksum.
The maximum mobile originated SBD message length is specified at 340 bytes. The minimum mobile
originated SBD message length is 1 byte. If the <SBD message length> parameter is out of range,
the 9602 issues response 3 (see below).
Once the DTE receives the READY indication from the 9602, the SBD message must be sent from
the DTE formatted as follows:
{binary SBD message} + {2-byte checksum}
The checksum is the least significant 2-bytes of the summation of the entire SBD message. The
high order byte must be sent first. For example if the DTE were to send the word “hello” encoded
in ASCII to the 9602 the binary stream would be hex 68 65 6c 6c 6f 02 14.
The mobile originated buffer will be empty upon power-up.
If any data is currently in the mobile originated buffer, it will be overwritten.*/
{
}

void ATSBDI()
/*Envoie ou recois message depuis/dans les buffer vers/depuis le gSS 

Note: The +SBDI command is provided for backwards compatibility with existing FAs which do not use
SBD Automatic Notification functionality. For SBD calls invoked with this command:
The SBD Session Type is fixed at type 0 – MO call.
The SBD Delivery Short Code will be the value specified by the +SBDDSC command.
The Detach/Attach flag is fixed at the Detach setting.
The Location Update flag is fixed at the No Update setting.
FAs requiring SBD Automatic Notification functionality should use the extended +SBDIX command.
Exec Command: +SBDI
This command initiates an SBD session between the 9602 and the GSS. If there is a message in the
mobile originated buffer it will be transferred to the GSS. Similarly if there is one or more messages
queued at the GSS the oldest will be transferred to the 9602 and placed into the mobile terminated buffer.
The message, if any, in the mobile originated buffer will be sent from the 9602 to the GSS.
If there is a MT-SBD message queued at the GSS it will be transferred to the 9602 and placed into
the mobile terminated buffer.
Command Response:
+SBDI:<MO status>,<MOMSN>,<MT status>,<MTMSN>,<MT length>,<MT queued>
where:
<MO status>:
MO session status provides an indication of the disposition of the mobile originated transaction. The field
can take on the following values:
0 No SBD message to send from the 9602.
1 SBD message successfully sent from the 9602 to the GSS.
2 An error occurred while attempting to send SBD message from 9602 to GSS.
<MOMSN>:
The Mobile Originated Message Sequence Number (MOMSN) is a value assigned by the 9602 when
sending a mobile-originated message to the GSS. This value is incremented each time an SBD session is
successfully completed between the 9602 to the GSS. This wrap around counter can range from 0 to
65535.
<MT status>:
The MT status provides an indication of the disposition of the mobile terminated transaction. The field can
take on the following values:
0 No SBD message to receive from the GSS.
1 SBD message successfully received from the GSS.
2 An error occurred while attempting to perform a mailbox check or receive a message from
the GSS.
<MTMSN>:
The Mobile Terminated Message Sequence Number (MTMSN) is assigned by the GSS when sending a
message to the 9602. This value is indeterminate if the field <MT status> is zero. This wrap around
counter can range from 0 to 65535.
Iridium Communications Inc.
Proprietary & Confidential Information
44
Distribution of Guide Restricted
to Product Developers OnlyIridium Communications Inc.
9602 SBD Transceiver Product Developers Guide
Information Contained in this Guide
is Subject to Change Without Notice
Revision 6.0
<MT length>:
The MT length is the length in bytes of the mobile terminated SBD message received from the GSS. If no
message was received, this field will be zero.
<MT queued>:
MT queued is a count of mobile terminated SBD messages waiting at the GSS to be transferred to the
9602.*/
{
}

int AT()
{
    // Renvoie 1 si ok 0 sinon
    return 1;
}




int main()
{
    fifo_recv = initialiser();
    fifo_send = initialiser();

    while (true)
    {
        
        printEtat();

        switch (etat)
        {

        case SETUP:
            ATE0();
            ATSBDD2();
            ATSBDC();
            etat = READ_STATUS;
            break;

        case READ_STATUS:
            ATSBDS(status,1);
            if (status[2] = 0) {
                etat = PREPARE_WRITE;
            } else {
                etat = READ_MESG;
            }
            break;

        case READ_MESG:
            ATSBDRB(msg);
            //enfiler(fifo_recv,&msg);
            ATSBDD1();
            etat = PREPARE_WRITE;
            break;

        case PREPARE_WRITE:
            if (status[0] = 0 & estVide(fifo_recv)) {
                etat = WRITE_MESG;
            } else if (status[0] != 0) {
                sent_mesg_count ++;
                etat = CLEAN_WRITE_BUFFER;
            } else {  
                etat = INIT_SESSION;
            }
            break;

        case CLEAN_WRITE_BUFFER:
            ATSBDD0();
            etat = PREPARE_WRITE;
            break;

        case WRITE_MESG:
            //msg = defiler(fifo_send);
            sent_mesg_count = status[1];
            etat = INIT_SESSION;
            break;

        case INIT_SESSION:
            ATSBDI();
            etat = CHECK_ALIVE;
            break;

        case CHECK_ALIVE:
            if (AT()) {
                etat = READ_STATUS;
            } else {
                etat = INIT_SESSION;
            }

            break;
        }

       
        sleep(1);
        i++;
    }
}
