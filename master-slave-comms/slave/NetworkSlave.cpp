#include "NetworkSlave.hpp" 

std::string NetworkSlave::Message;

void* NetworkSlave::Task(void *arg)
{
  int n;
  int newsockfd=(int)arg;
  char msg[MAXPACKETSIZE];
  pthread_detach(pthread_self());
  while(1)
  {
    n=recv(newsockfd,msg,MAXPACKETSIZE,0);
    if(n==0)
    {
      close(newsockfd);
      break;
    }
    msg[n]=0;
    //send(newsockfd,msg,n,0);
    Message = std::string(msg);
  }
  return 0;
}

void NetworkSlave::setup(int port)
{
  sockfd=socket(AF_INET,SOCK_STREAM,0);
  std::memset(&serverAddress,0,sizeof(serverAddress));
  serverAddress.sin_family=AF_INET;
  serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
  serverAddress.sin_port=htons(port);
  bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));
  listen(sockfd,5);
}

std::string NetworkSlave::receive()
{
  std::string str;
  while(1)
  {
    socklen_t sosize  = sizeof(clientAddress);
    newsockfd = accept(sockfd,(struct sockaddr*)&clientAddress,&sosize);
    str = inet_ntoa(clientAddress.sin_addr);
    pthread_create(&serverThread,NULL,&Task,(void *)newsockfd);
  }
  return str;
}

std::string NetworkSlave::getMessage()
{
  return Message;
}

void NetworkSlave::Send(std::string msg)
{
  send(newsockfd,msg.c_str(),msg.length(),0);
}

void NetworkSlave::clean()
{
  Message = "";
  std::memset(msg, 0, MAXPACKETSIZE);
}

void NetworkSlave::detach()
{
  close(sockfd);
  close(newsockfd);
}

