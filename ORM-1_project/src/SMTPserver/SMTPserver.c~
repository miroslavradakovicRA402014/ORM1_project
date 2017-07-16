#include "SMTPserverdefines.h"

static char      SMTPmessage[MESG_LEN];
static char      spamSMTPmessage[MESG_LEN];
static int       clientSocket[MAX_CLIENT];
static int       SMTPserverSocket[MAX_SOCK];
static struct    sockaddr_in SMTPserver,client[MAX_CLIENT],server;
static E_MAIL    eMail;
static int       clientId = 0;
static char*     emailAddresses[MAX_ADDR];
static FILE*     filePointer;
static int       adrInd;
static pthread_t hClientProxy[MAX_CLIENT];
static pthread_t hSMTPclient; 
static pthread_t hSMTPserver;
static sem_t     semaphores[MAX_SEM];
static char*     serverAddressIP;
static char*     SMTPserverAddressIP;

//Create SMTP sockets,one for communication with client and other one for server  
int CreateSocket()
{
    SMTPserverSocket[0] = socket(AF_INET,SOCK_STREAM,0);
    if (SMTPserverSocket[0] == -1)
    {
        perror("Could not create SMTP proxy server socket\n");
        return -1;
    }
    puts("SMTP proxy server socket created");
    
    SMTPserver.sin_addr.s_addr = inet_addr(SMTPserverAddressIP);
    SMTPserver.sin_family = AF_INET;
    SMTPserver.sin_port = htons(PORT_NUM_C);

    SMTPserverSocket[1] = socket(AF_INET,SOCK_STREAM,0);
    if (SMTPserverSocket[1] == -1)                      
    {
        perror("Could not create SMTP server socket\n");
        return -1;
    }
    puts("SMTP proxy server socket created");
    
    server.sin_addr.s_addr = inet_addr(serverAddressIP);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT_NUM_S);


    return 1;
}
//Close SMTP server sockets
int CloseSocket(int sock)
{
    if (close(sock) < 0) 
    {
        perror("Could not close SMTP proxy server server socket\n");
        return -1;
    }
    puts("SMTP proxy server socket closed");

    return 1;
}
//Listen connections form client
int ListenClientConnections()
{
    if (listen(SMTPserverSocket[0],MAX_CLIENT) < 0)
    {
        perror("Listen connections failed\n");
        return -1;
    }
 
    return 1;
}
//Accept connections from clients
int AcceptClientConnections(int ind)
{
    int c = sizeof(struct sockaddr_in);

    clientSocket[ind] = accept(SMTPserverSocket[0],(struct sockaddr *)&client[ind],(socklen_t*)&c);
    if (clientSocket[ind] < 0)
    {
       perror("Accept form SMTP proxy server failed\n");
       return -1;
    } 
    puts("\nConnection form client accepted");

    return 1;
}
//Bind connection
int BindSocket()
{
    if(bind(SMTPserverSocket[0],(struct sockaddr*)&SMTPserver,sizeof(SMTPserver)) < 0)
    {
       perror("Bind failed\n");
       return -1;
    }
    puts("Bind done");

    return 1;
}
//Recive message form client
int  ReciveClientSMTPMessage(int ind)
{
    int readSize;

    if ((readSize = recv(clientSocket[ind],(E_MAIL*)&eMail,sizeof(E_MAIL),0)) > 0 )
    {
        printf("Message from: %s \n",eMail.emailAddress);
        printf("Message: %s\n",eMail.emailData);
    }

    if(readSize == 0)
    {
        puts("Client disconnected \n");
        fflush(stdout);
        return 1;
    }
    else if(readSize == -1)
    {
        perror("Transfer form client failed \n");
        return -1;
    }
   
    return 1;
}
//Send message to client
int SendClientSMTPMessage(int ind)
{  
    if(send(clientSocket[ind],(char*)&SMTPmessage,sizeof(SMTPmessage),0) < 0)
    {
        perror("SMTP client message send failed");
        return -1;
    }

    return 1;
}
//Recive message form server
int ReciveServerSMTPMessage()
{
    int readSize;

    if ((readSize = recv(SMTPserverSocket[1],(char*)SMTPmessage,MESG_LEN,0)) > 0)
    {
        return 1;
    }
    perror("Transfer form e-mail server failed \n");
    strcpy(SMTPmessage,"Mail has not been sent to ");
    strcat(SMTPmessage,eMail.emailAddressReciver);

    return -1;
}
//Forward client message to server
int SendServerSMTPMessage()
{
    if(send(SMTPserverSocket[1],(E_MAIL*)&eMail,sizeof(E_MAIL),0) < 0)
    {
        perror("Send to e-mail server failed\n");
        return -1;
    }

    return 1;
}
//Connect to e-mail server
int ConnectToServer()
{
    if (connect(SMTPserverSocket[1],(struct sockaddr *)&server,sizeof(server)) < 0)
    {
        perror("Connection to  e-mail server failed\n");
        return -1;
    }
    puts("Connected to e-mail server");

    return 1;
}
//Send spam message to client if address is spam
int SendSpamSMTPMessage(int ind)
{
    strcpy(spamSMTPmessage,eMail.emailAddress);
    strcat(spamSMTPmessage," is spam address,can not access to e-mail server");
  
    if(send(clientSocket[ind],(char*)&spamSMTPmessage,sizeof(spamSMTPmessage),0) < 0)
    {
        perror("SMTP message send failed");
        return -1;
    }

    return 1;
}
//Open file with spam addresses
int FileOpen()
{
    filePointer = fopen("addresses.txt","r");

    if (filePointer == NULL)
    {
        perror("File addresses.txt open failed\n");
        return -1;
    }
    puts("File opened");

    return 1;
}
//Read spam addresses from file
int ReadAddressesFromFile()
{
    adrInd = 0;
    while (!feof(filePointer))
    {
        emailAddresses[adrInd] = (char*)malloc(EMAIL_ADDR_LEN);   
        if (emailAddresses[adrInd] == NULL)
        {
            perror("Don't enough RAM\n");
            return -1;
        }     
        fscanf(filePointer," %s ",emailAddresses[adrInd]);
        adrInd++;
    }
    return 1;
}
//Address in file?
int AddressInFile(char* address)
{
    int i;
    for (i = 0; i < adrInd ; i++)
    {
        if (!strcmp(address,emailAddresses[i]))
        {
            return 1;
        }
    }
    return 0;
}
//Free memory
void FreeHeap()
{
    int i;
    for (i = 0; i < adrInd ; i++)
    {
        free(emailAddresses[i]);
    }  
}
//Thread who handle communication with client 
void* ClientProxyThread(void *pParam)
{
    if (AcceptClientConnections(clientId) < 0)
    {
        return NULL;
    }

    if (ReciveClientSMTPMessage(clientId) < 0)
    {
        return NULL;
    }

    if (AddressInFile(eMail.emailAddress))
    {
        SendSpamSMTPMessage(clientId);
        return NULL;
    }

    sem_post(&semaphores[1]);

    return NULL;
}
//Thread who accept client connections
void* SMTPclientThread(void *pParam)
{
    while (1)
    {
        if (clientId == MAX_CLIENT)//Check client number is greater than max number of clients 
        {
           perror("SMTP proxy server broke down.Error\n");
           return NULL;
        }
        pthread_create(&hClientProxy[clientId],NULL,ClientProxyThread,0);
        pthread_join(hClientProxy[clientId], NULL);
        sem_wait(&semaphores[0]);//Wait to server recive message
        clientId++;
    }
}
//Thread who handle communication with server
void* SMTPserverThread(void *pParam)
{
    while (1)
    {
        sem_wait(&semaphores[1]);//Wait to SMTP recive message from client
        SendServerSMTPMessage();
        ReciveServerSMTPMessage(); 
        SendClientSMTPMessage(clientId);          
        sem_post(&semaphores[0]);//Ping thread who wait to server send message
    }
    return NULL;
}

int main(int argc , char *argv[])
{  
    SMTPserverAddressIP = argv[1];
    serverAddressIP = argv[2];

    if (FileOpen() < 0)
    {
        return -1;
    }
 
    if (ReadAddressesFromFile() < 0)
    {
        return -1;
    }

    if (CreateSocket() < 0)
    {
        return -1;
    }

    if (BindSocket() < 0)
    {
        return -1;
    }

    if (ListenClientConnections() < 0)
    {
        return -1;
    }

    if (ConnectToServer() < 0)
    {
        return -1;
    } 
    
    sem_init(&semaphores[0],1,0);
    sem_init(&semaphores[0],0,0);
 
    pthread_create(&hSMTPserver,NULL,SMTPserverThread,0);
    pthread_create(&hSMTPclient,NULL,SMTPclientThread,0);
    pthread_join(hSMTPclient,NULL);
    pthread_join(hSMTPserver,NULL);    
    
    sem_destroy(&semaphores[0]);
    sem_destroy(&semaphores[1]);

    FreeHeap();
    fclose(filePointer);
  
    if (CloseSocket(SMTPserverSocket[0]) < 0)
    {
        return -1;
    }

    if (CloseSocket(SMTPserverSocket[1]) < 0)
    {
        return -1;
    }
    

    return 0;
}
