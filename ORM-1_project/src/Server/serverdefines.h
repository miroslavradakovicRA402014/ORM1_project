#include <stdio.h>
#include <string.h>    
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <unistd.h> 

#define BUFLEN         512
#define PORT_NUM       27025
#define EMAIL_ADDR_LEN 50
#define MESG_LEN       80

int  CreateSocket();
int  CloseSocket(int sock);
int  BindSocket();
int  ListenConnection();
int  AcceptConnection();
int  ReciveSMTPMessage();
int  SendSMTPMessage();

typedef struct {
  
    char emailAddress[EMAIL_ADDR_LEN];
    char emailAddressReciver[EMAIL_ADDR_LEN];
    char emailData[BUFLEN];
  
} E_MAIL;