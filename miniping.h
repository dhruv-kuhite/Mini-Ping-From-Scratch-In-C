#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include<process.h>

WSADATA wsa_data;

LARGE_INTEGER Freq;

int numberofPing=4;

typedef struct icmp_header
{
   unsigned char Type;
   unsigned char Code;
   unsigned short CheckSum;
   unsigned short Id;
   unsigned short squence;
   unsigned char Payload[32];
} icmp_header;

typedef struct
{

   unsigned char Hel : 4;
   unsigned char ver : 4;

   unsigned char serviceT;

   unsigned short TotalLength;

   unsigned short DatagramIdentification;

   unsigned short fragmentdata : 13;
   unsigned short flag : 3;

   unsigned char ttl;

   unsigned char protocol;

   unsigned short checksum;

   unsigned int sourceip;
   unsigned int destinationip;

} ipv4_header;


void setTimeOut(SOCKET sockfd);
INT64 GetTime();



void Build_HeaderAandPayload();
void bindSource(int sockfd);
unsigned short CheckSum(unsigned char *data, int len);

void SendEchoRequest(SOCKET sockfd);
void EchoReply(SOCKET sockfd);

void DisplayInformation(int sockfd, char buffer[], int sizeofBuffer, int result);