#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    int sockfd;
    struct sockaddr_in local_addr, remote_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    char buffer[256];
    if(argc < 2) {
        printf("Use: <listening_PORT>\n");
        return 0;
    }

    if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Error in socket opening ... exit!");
        return -1;
    }

    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    //local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sockfd, (struct sockaddr*) &local_addr, sizeof(local_addr)) < 0) {
        printf("Binding error! Error code n. %d\n", errno);
        return -1;
    }

    while(1) {
        int num = 0;
        if((num = recvfrom(sockfd, buffer, 99, 0, (struct sockaddr*) &remote_addr, &len)) < 0) {
            printf("Error in receiving message...\n");
        }
        buffer[num] = '\0'; 
        printf("Packet from IP: %s PORT: %d Msg: %s\n", inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port), buffer);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*) &remote_addr, len);
        memset(buffer,0,256);
        //printf("Packet from IP: %s PORT: %s\n", inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port));
    }
}
