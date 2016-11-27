#include<stdio.h>
#include<string.h> //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
int main(int argc , char *argv[])
{
int socket_desc,port;
struct sockaddr_in server;
char message[50] , server_reply[2000];
char IP[20];
//Create socket
socket_desc = socket(AF_INET , SOCK_STREAM , 0);
if (socket_desc == -1)
{
printf("Could not create socket");
}
printf("輸入IP:");
scanf("%s",IP);
printf("輸入port:");
scanf("%d",&port);
server.sin_addr.s_addr = inet_addr(IP);
server.sin_family = AF_INET;
server.sin_port = htons( port );
//Connect to remote server
if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
{
puts("connect error");
return 1;
}
puts("Connected\n");
while(1)
{
//Send some data
//message = "GET / HTTP/1.1\r\n\r\n";
printf("\n輸入4個不同數字:");
scanf("%s",message);
if( send(socket_desc , message , strlen(message) , 0) < 0)
{
puts("Send failed");
return 1;
}
puts("Data Send\n");
//Receive a reply from the server
if( recv(socket_desc, server_reply , 2000 , 0) < 0)
{
puts("recv failed");
}
puts("Reply received\n");
puts(server_reply);
if(strlen(server_reply)==15 || strlen(message)==1)
return 0;
}
return 0;
}
