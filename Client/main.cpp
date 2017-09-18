#include <ws2tcpip.h>
#include <iostream>
#include <winsock2.h>
#define PORT 7770
#define SERVERADDR "127.0.0.1"
#include <colordlg.h>
#include <conio.h>
#include <windows.h>
#include <stdio.h>
#include <clocale>
#include <limits.h>



using namespace std;

SOCKET Connect;

char* rus(char* st)
{
    char* p = st;
    while( *p ){
        if(*p >= 192)
            if(*p <= 239)
                *p -= 64;
            else
                *p -= 16;
        p++;
    }
    return st;
}

void ReadMessageFromServer()
{
    char * buffer;
    for ( ; ; Sleep(75))
    {
        buffer = new char[1024];
        //memset(buffer, 0, 1024);

        if (recv(Connect, buffer, 1024, 0)) //!=SOCKET_ERROR)
        {
            printf("%s",buffer);
        }
        //free(buffer);
        //delete[] buffer;
    }
    free(buffer);
    //delete[] buffer;
}

void WriteMessageToServer(char *name)
{
    char * buffer;
    int namelen = 0;
    while(name[namelen]) namelen++;
    int b = namelen;
    char res[1024] = "";
    for ( ; ; Sleep(75))
    {
        buffer = new char[1024];
        memset(buffer, 0, 1024);
        fgets(buffer,1024, stdin);
        rus(buffer);
        if (!strcmp(&buffer[0],"quit\n"))
        {
            printf("Exit...");
            closesocket(Connect);
            WSACleanup();
            return;
        }
        int bufferlen = 0;
        while(buffer[bufferlen]) bufferlen++;
        int count = 0;
        int count2 = 0;
        while(namelen!=0){
            res[count] = name[count];
            count++;
            namelen--;
        }
        while(bufferlen!=0){
            res[count] = buffer[count2];
            count++;
            count2++;
            bufferlen--;
        }
        send(Connect,res,1024,0);
        int clear = 0;
        while(res[clear]){
            res[clear] = 0;
            clear++;
        }
        int clear2 = 0;
        while(buffer[clear2]){
            res[clear2] = 0;
            clear2++;
        }
        free(buffer);
        delete[] buffer;
        namelen = b;
    }
    free(buffer);
    delete[] buffer;
}




int main()
{
    WSAData data;
    WORD version = MAKEWORD(2,2);
    int res = WSAStartup(version,&data);
    if(res!=0) {
        return 0;
    }
    Connect = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in hos_addr;
    hos_addr.sin_family=AF_INET;
    hos_addr.sin_port=htons(PORT);
    HOSTENT *hostt;
    //if (inet_addr(SERVERADDR)!=INADDR_NONE)//������������ ��ப� � ���祭��,��堭��� ��� ��� �訡��
    //hos_addr.sin_addr.s_addr=inet_addr(SERVERADDR);
    //else
    if (hostt=gethostbyname(SERVERADDR)){
        ((unsigned long *)&hos_addr.sin_addr)[0]=((unsigned long **)hostt->h_addr_list)[0][0];
    }
    else
    {
        printf("Wrong adress %s\n",SERVERADDR);
        closesocket(Connect);
        WSACleanup();
        return -1;
    }
    if (connect(Connect,(sockaddr *)&hos_addr,sizeof(hos_addr)))
    {
        printf("Connect error %d\n",WSAGetLastError());
        return -1;
    }
    printf("Connection on chat server: %s is stable\n\n",SERVERADDR);
    printf("Chat client ver. 0.1\n");
    printf("\n\nlogin:");
    char name[1024] = "";
    char pwd[1024] = "";
    fgets(name,1024, stdin);
    printf("password:");
    int i = 0;
    for(i=0;(pwd[i] = getch()) != '\r';)   {
        if(pwd[i]=='\b' && i!=0) {
            printf("%s", "\b \b");
            i--;
        }
        else if(pwd[i]!='\b') {
            printf("%c", '*');
            i++;
        }
    }
    pwd[i] = '\n';
    i++;
    pwd[i] = '\0';
    printf("\n");
    rus(name);
    int namelen = 0;
    while(name[namelen])namelen++;
    char res2[1024] = "";
    int pwdlen = 0;
    while(pwd[pwdlen]) pwdlen++;
    int count = 0;
    int count2 = 0;
    while(namelen!=0){
        res2[count] = name[count];
        count++;
        namelen--;
    }
    while(pwdlen!=0){
        res2[count] = pwd[count2];
        count++;
        count2++;
        pwdlen--;
    }
    char buff[1024];
    memset(buff,0,1024);
    for(;;Sleep(75))
    {
        if(recv(Connect,buff, 1024,0) !=SOCKET_ERROR){
            CreateThread(0,0,(LPTHREAD_START_ROUTINE)ReadMessageFromServer,0,0,0);
            CreateThread(0,0,(LPTHREAD_START_ROUTINE)WriteMessageToServer,(LPVOID)res2,0,0);
            memset(buff,0,sizeof(buff));
            //free(buff);
        }
    }
    return 1;
}
