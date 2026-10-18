#include "miniping.h"

struct sockaddr_in destination;
struct sockaddr_in source;

char *destinationAdress = "8.8.8.8";
icmp_header icmp_request;
icmp_header icmp_reply;
ipv4_header ip4_header;

struct sockaddr_in sender;
int Sender_Size = sizeof(sender);

unsigned short CheckSum(unsigned char *data, int len)
{

    unsigned char *bytes = data;

    unsigned int sum = 0;

    for (int i = 0; i < len; i += 2)
    {

        if (i + 1 < len)
            sum += (bytes[i] << 8) + bytes[i + 1];
        else
            sum += (bytes[i] << 8);
    }

    unsigned int overflow, underflow;
    while (sum > 0xFFFF)
    {
        overflow = sum >> 16;
        underflow = sum & 0xFFFF;
        sum = underflow + overflow;
    }

    return (unsigned short)~sum;
}

void setTimeOut(SOCKET sockfd)
{

    int TimeOut = 4000;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut));
}

INT64 GetTime()
{
    LARGE_INTEGER counter;

    QueryPerformanceCounter(&counter);

    return (counter.QuadPart * 1000 / Freq.QuadPart);
}

void Build_HeaderAandPayload()
{
    memset(&destination, 0, sizeof(destination));
    destination.sin_family = AF_INET;

    destination.sin_addr.S_un.S_addr = inet_addr(destinationAdress);

    // icmp request header values
    memset(&icmp_request, 0, sizeof(icmp_request));
    icmp_request.Type = 8;
    icmp_request.Code = 0;
    icmp_request.Id = htons(getpid() & 0xFFFF);
    icmp_request.squence = htons(1);
    icmp_request.CheckSum = 0;
    int size = 0;

    INT64 time = GetTime();
    memcpy((void *)&icmp_request.Payload[0], &time, sizeof(time));
    size += sizeof(time);

    for (int i = 0; size < sizeof(icmp_request.Payload); size++, i++)
    {
        icmp_request.Payload[size] = (i % 26) + 'A';
    }

    icmp_request.CheckSum = htons(CheckSum((unsigned char *)&icmp_request, sizeof(icmp_request)));
}

void bindSource(int sockfd)
{

    source.sin_family = AF_INET;
    source.sin_addr.S_un.S_addr = INADDR_ANY;
    int result = bind(sockfd, (struct sockaddr *)&source, sizeof(source));

    if (result != 0)
    {
        printf("Bind Failed\n");
    }
    else
    {
        printf("bind success\n");
    }
}

void SendEchoRequest(SOCKET sockfd)
{

    int result = sendto(sockfd,
                        (char *)&icmp_request,
                        sizeof(icmp_request),
                        0,
                        (struct sockaddr *)&destination,
                        sizeof(destination));

    if (result == SOCKET_ERROR)
    {
        printf("sendto : failed to send data\n");
    }
 
}

void EchoReply(SOCKET sockfd)
{

    unsigned char buffer[200];
    memset(&buffer, 0, sizeof(buffer));

    int result = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&sender, &Sender_Size);

    // if (result == SOCKET_ERROR)
    // {
    //     printf("recvfrom : unable to recive data  %d", WSAGetLastError());
     
        
    //     return;
    // }

    int size = 20;

    memset((void *)&ip4_header, 0, sizeof(ipv4_header));
    memcpy((void *)&ip4_header, (void *)&buffer[0], size);

    memset((void *)&icmp_reply, 0, sizeof(icmp_reply));
    memcpy((void *)&icmp_reply, &buffer[size], sizeof(icmp_reply));

    DisplayInformation(sockfd,buffer,sizeof(buffer),result);

    return;
}

void DisplayInformation(int sockfd, char buffer[], int sizeofBuffer, int result)
{
    if (icmp_reply.Id != icmp_request.Id)
    {
        memset(buffer, 0, sizeofBuffer);
        recvfrom(sockfd, buffer, sizeofBuffer, 0, (struct sockaddr *)&sender, &Sender_Size);
    }

    if (result == SOCKET_ERROR && WSAGetLastError()==WSAETIMEDOUT)
    {
        printf("Request TimedOut\n");
        return;
    }

    if (icmp_reply.Type == 3)
    {
        switch (icmp_reply.Code)
        {
        case 0:
            printf("Net UnRechable\n");
            break;

        case 1:
            printf("Host UnRechable\n");
            break;

        case 2:
            printf("Protocol UnRechable\n");
            break;

        case 3:
            printf("Port UnRechable\n");
            break;

        case 4:
            printf("Fragmentation Needed\n");
            break;

        case 5:
            printf("Source Route Failed\n");
            break;

        case 6:
            printf("Destination Network Unknown\n");
            break;

        case 7:
            printf("Destination Host Unknown\n");
            break;

        case 8:
            printf("Source Host Isolated\n");
            break;

        case 9:
            printf("Network Prohibited\n");
            break;

        case 10:
            printf("Host Prohibited\n");
            break;

        case 11:
            printf("Network Unreachable for ToS\n");
            break;

        case 12:
            printf("Host Unreachable for ToS\n");
            break;

        case 13:
            printf("Communication Prohibited |\n");
            break;

        case 14:
            printf("Host Precedence Violation\n");
            break;

        case 15:
            printf("Precedence Cutoff in Effect\n");
            break;
        }

       return;
    }


    if (icmp_reply.Type==0)
    {
        
        INT64 currenttime=GetTime();
        INT64 pasttime;
        memcpy(&pasttime,&icmp_reply.Payload[0],8);
          
        
        printf("Reply from %s bytes=32 TTL=%d time:%ld ms\n",destinationAdress,ip4_header.ttl,currenttime-pasttime);

    }
    

    return;
}

int main(int totalargs,char *args[])
{

    if (totalargs<2  || totalargs>3)
    {
        printf("Enter like this : miniping [ip]\n");
         printf("or like this : miniping [ip] [Number of ping]\n");
        return -1;
    }
    
    else  if (totalargs==2)
    {
        destinationAdress=args[1];
    }
     else  if (totalargs==3)
    {
        destinationAdress=args[1];
        numberofPing=atoi(args[2]);
    }
    

    QueryPerformanceFrequency(&Freq);

    // Allow us to use Socket From winshock ver 2.2
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);

    if (result != 0)
    {
        printf("Winshok unable to initilize : %d", result);
        return -1;
    }

    printf("WinShock Initilized\n");

    SOCKET sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (sockfd == INVALID_SOCKET)
    {
        printf("Failed To initialize Socket\n");
        WSACleanup();
        return -1;
    }

    printf("ICMP SOCKET CREATED\n");

    setTimeOut(sockfd);

    
    bindSource(sockfd);


     printf("pinging %s with 32 bytes of Data......\n", destinationAdress);
    // number of pings

    for (int i = 1; i <= numberofPing; i++)
    {
        Sleep(1000);
        Build_HeaderAandPayload();

         SendEchoRequest(sockfd);

    EchoReply(sockfd);
    
    }
    
      

    closesocket(sockfd);
    WSACleanup();
    return 0;
}