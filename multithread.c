#include<stdio.h>
#include<string.h> //strlen
#include<stdlib.h> //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h> //write
#include<pthread.h> //for threading , link with lpthread
#include <time.h>
void *connection_handler(void *);
int main(int argc , char *argv[])
{
int socket_desc , new_socket , c , *new_sock;
struct sockaddr_in server , client;
char *message;
//Create socket
socket_desc = socket(AF_INET , SOCK_STREAM , 0);
if (socket_desc == -1)
{
printf("Could not create socket");
}
//Prepare the sockaddr_in structure
server.sin_family = AF_INET;
server.sin_addr.s_addr = INADDR_ANY;
server.sin_port = htons( 8888 );
//Bind
if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
{
puts("bind failed");
return 1;
}
puts("bind done");
//Listen
listen(socket_desc , 3);
//Accept and incoming connection
puts("Waiting for incoming connections...");
c = sizeof(struct sockaddr_in);
while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
{
puts("Connection accepted");
//Reply to the client
message = "Hello Client , I have received your connection. And now I will assign a handler for you\n****輸入任意1數字後即可離開****\n\n";
write(new_socket , message , strlen(message));
pthread_t sniffer_thread;
new_sock = malloc(1);
*new_sock = new_socket;
if( pthread_create( &sniffer_thread , NULL , connection_handler , (void*) new_sock) < 0)
{
perror("could not create thread");
return 1;
}
//Now join the thread , so that we dont terminate before the thread
//pthread_join( sniffer_thread , NULL);
puts("Handler assigned");
}
if (new_socket<0)
{
perror("accept failed");
return 1;
}
return 0;
}
/*
* This will handle connection for each client
* */
void *connection_handler(void *socket_desc)
{
//Get the socket descriptor
int sock = *(int*)socket_desc;
int read_size;
char *Amessage , client_message[2000];
//
int n;
char buffer[256];
time_t mytime;
mytime=time(NULL);
int answer[8];//猜數答案和使用者的回答
int count1,count2,countA,countB;//記數用變數
char reply[8],compare[8],message2[50];
countA=0;
countB=0;
srand( time(NULL) );
for(count1=0;count1<4;count1++)
{
answer[count1]=rand()%10; //亂數產生1~10任意數字並放入陣列內
for(count2=0;count2<count1;count2++)
{
if(answer[count1]==answer[count2]) //比較是否重複
{
count1--; //重複則重新產生
}
}
}
printf("Answer:%d%d%d%d\n",answer[0],answer[1],answer[2],answer[3]);
//
//Send some messages to the client
// bzero(message,2000);
// message = "Greetings! I am your connection handler\n";
// write(sock , message , strlen(message));
// bzero(message,2000);
// message = "Now type something and i shall repeat what you type \n";
// write(sock , message , strlen(message));
//Receive a message from client
// while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
while(1)
{
//Send the message back to client
//write(sock , client_message , strlen(client_message));
bzero(client_message,2000);
n = read(sock,client_message,2000);
compare[0]=(client_message[0]-'0');
compare[1]=(client_message[1]-'0');
compare[2]=(client_message[2]-'0');
compare[3]=(client_message[3]-'0');
for(count1=0;count1<4;count1++)
{
if(answer[count1]==compare[count1]) //比較數字及位置相同的數
{
countA+=1;
}
}
for(count2=0;count2<=3;count2++)
{
for(count1=0;count1<=3;count1++)
{
if(compare[count1]==answer[count2] && count1!=count2)
{
countB++;
}
}
}
if(countA==4)
{
bzero(client_message,2000);
Amessage="This game end!!";
write(sock , Amessage, strlen(Amessage)+1);
}
else
{
bzero(client_message,2000);
sprintf(message2,"%dA%dB",countA,countB);
write(sock , message2, strlen(message2)+1);
countA=0;countB=0;
}
}
if(read_size == 0)
{
puts("Client disconnected");
fflush(stdout);
}
else if(read_size == -1)
{
perror("recv failed");
}
//Free the socket pointer
free(socket_desc);
return 0;
}
