#include "serverdefines.h"

static int    serverSocket;
static int    SMTPserverSocket;
static struct sockaddr_in server,SMTPserver;
static E_MAIL eMail;
static char   SMTPmessage[MESG_LEN];
static char*  serverAddressIP;

//Create server socket
int CreateSocket()
{
    serverSocket = socket(AF_INET,SOCK_STREAM,0);

    if (serverSocket == -1)
    {
        perror("Could not create e-mail server socket\n");
        return -1;
    }
    puts("E-mail server socket created");
  
    server.sin_addr.s_addr = inet_addr(serverAddressIP);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT_NUM);

    return 1;
}
//Close server socket 
int CloseSocket(int sock)
{
    if (close(sock) < 0) 
    {
        perror("Could not close e-mail server socket\n");
        return -1;
    }
    puts("E-mail server socket closed");

    return 1;
}
//Bind connection
int BindSocket()
{
    if(bind(serverSocket,(struct sockaddr*)&server,sizeof(server)) < 0)
    {
        perror("Bind failed\n");
        return -1;
    }
    puts("Bind done");

    return 1;
}
//Listen connections form SMTP server
int ListenConnection()
{
    if (listen(serverSocket,1) < 0)
    {
        perror("Listen connection from SMTP proxy\n");
        return -1;
    }

   return 1;
}
//Accept connection form client
int AcceptConnection()
{
    puts("Waiting for connection from SMTP proxy server...");
    int c = sizeof(struct sockaddr_in);

    SMTPserverSocket = accept(serverSocket,(struct sockaddr *)&SMTPserver,(socklen_t*)&c);
    if (SMTPserverSocket < 0)
    {
        perror("Accept form e-mail server failed\n");
        return -1;
    }
    puts("Connection form e-mail server accepted");

    return 1;
}
//Send SMTP message to client
int SendSMTPMessage()
{
    strcpy(SMTPmessage,"Mail has been sent to ");
    strcat(SMTPmessage,eMail.emailAddressReciver);
   
    if(send(SMTPserverSocket,(char*)&SMTPmessage,sizeof(SMTPmessage),0) < 0)
    {
        perror("E-mail SMTP message send failed");
        return -1;
    }

    return 1;
}
//Recive SMTP message form client
int ReciveSMTPMessage()
{
    int readSize;

    while (1)
    {
       if ((readSize = recv(SMTPserverSocket,(E_MAIL*)&eMail,sizeof(E_MAIL),0)) > 0 )
       {
           printf("\nMail from: %s \n",eMail.emailAddress);
           printf("%s \n",eMail.emailData);
       }

       if (SendSMTPMessage() < 0)
       {
           perror("Send SMTP message to SMTP proxy failed\n");
       }   

       if(readSize == 0)
       {
           puts("SMTP proxy server disconnected \n");
           fflush(stdout);
           return 1;
       }
       else if(readSize == -1)
       {
           perror("Transfer form SMTP proxy server failed \n");
           return -1;
       }
    }
   
    return 1;
}


int main(int argc,char *argv[])
{  
    serverAddressIP = argv[1];

    if (CreateSocket() < 0)
    {
        return -1;
    }
   
    if (BindSocket() < 0)
    {
        return -1;
    }

    if (ListenConnection() < 0)
    {
        return -1;
    }

    if (AcceptConnection() < 0)
    {
        return -1; 
    }

    if (ReciveSMTPMessage() < 0)
    {
        return -1;
    }

    return 0;
}
