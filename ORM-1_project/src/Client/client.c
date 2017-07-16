#include "clientdefines.h"

static int    clientSocket;            
static struct sockaddr_in server;
static E_MAIL eMail;
static char   serverMessage[MESG_LEN];
static char*  serverAddressIP;

//Create client socket 
int CreateSocket()
{
    clientSocket = socket(AF_INET,SOCK_STREAM,0);
    if (clientSocket == -1)
    {
        perror("Could not create client socket\n");
        return -1;
    }
  
    server.sin_addr.s_addr = inet_addr(serverAddressIP);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT_NUM);
 
    return 1;
}
//Connect to proxy server
int ConnectToSMTP()
{
    if (connect(clientSocket,(struct sockaddr *)&server,sizeof(server)) < 0)
    {
        perror("Connection to SMTP proxy server failed\n");
        return -1;
    }

    return 1;
}
// Send e-mail
int SendSMTPMessage()
{
    if(send(clientSocket,(E_MAIL*)&eMail,sizeof(E_MAIL),0) < 0)
    {
        perror("E-mail send failed\n");
        return -1;
    }

    return 1;
}
// Close client socket
int CloseSocket(int sock)
{
    if (close(sock) < 0) 
    {
        perror("Could not close client socket\n");
        return -1;
    }

    return 1;
}
// Recive server message
int ReciveSMTPMessage()
{
    int readSize;

    if ((readSize = recv(clientSocket,(char*)serverMessage,MESG_LEN,0)) > 0 )
    {
        printf("%s\n",serverMessage);
        return 1;
    }
    perror("Transfer form SMTP proxy server failed \n");

    return -1;
}

void ComposeMail()
{
    char ch;
    int ind;
    puts("Mail to:");

    strcpy(eMail.emailAddress,"miroslav.radakovic23@gmail.com");

    while ((ch = (char)getchar()) != '\n')
    {
        eMail.emailAddressReciver[ind] = ch;
        ind++;
    }
    eMail.emailAddressReciver[ind] = '\0';

    puts("Message:");
    ind = 0;
    while ((ch = (char)getchar()) != '\n')
    {
        eMail.emailData[ind] = ch;
        ind++;
    } 
    eMail.emailData[ind] = '\0';
}

int main(int argc,char *argv[])
{ 
    serverAddressIP = argv[1];

    ComposeMail();  
    
    if (CreateSocket() < 0)
    {
        return -1;
    }

    if (ConnectToSMTP() < 0)
    {
        return -1;
    }
    
    if (SendSMTPMessage() < 0)
    {
        return -1;
    }

    if (ReciveSMTPMessage() < 0)
    {
        return -1;
    }

    if (CloseSocket(clientSocket) < 0)
    {
        return -1;
    }
    
    return 0;
}
