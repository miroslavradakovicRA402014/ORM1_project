#include <stdio.h>
#include <string.h>    
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <unistd.h>  
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define BUFLEN         512
#define PORT_NUM_C     27015
#define PORT_NUM_S     27025
#define EMAIL_ADDR_LEN 50
#define MESG_LEN       80
#define MAX_CLIENT     10
#define MAX_ADDR       10
#define MAX_SEM        2
#define MAX_SOCK       2

int   CreateSocket();
int   CloseSocket(int sock);
int   ListenClientConnections();
int   AcceptClientConnections(int ind);
int   BindSocket();
int   ReadAddressesFromFile();
int   FileOpen();
int   AddressInFile(char* address);
int   ReciveClientSMTPMessage(int ind);
int   SendClientSMTPMessage(int ind);
int   SendServerSMTPMessage();
int   ReciveServerSMTPMessage();
int   SendSpamSMTPMessage(int ind);
int   ConnectToServer();
void  FreeHeap();
void* ClientProxyThread(void *pParam);
void* SMTPclientThread(void *pParam);
void* SMTPserverThread(void *pParam);


typedef struct {
  
    char emailAddress[EMAIL_ADDR_LEN];
    char emailAddressReciver[EMAIL_ADDR_LEN];
    char emailData[BUFLEN];
  
} E_MAIL;

