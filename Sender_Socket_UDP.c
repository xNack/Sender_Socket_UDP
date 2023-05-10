#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>



int main(int argc, char* argv[]) {
    int sockfd;
    struct sockaddr_in dest_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    //extern int errno;
    int i, n;
    char buffer[256];

    if(argc < 3) {
        printf("Use: sender IP_dest PORT_dest");
        return 0;
    }

    if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Error in socket opening ... exit!");
        return -1;
    }

    //Ci assicuriamo che la stuttura sia vuota!
    memset(&dest_addr, 0, sizeof(dest_addr));

    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &(dest_addr.sin_addr));
    dest_addr.sin_port = htons(atoi(argv[2]));

     n = 0;


    while(fgets(buffer, 256, stdin) != NULL) {
        // Invia un messaggio
        sendto(sockfd, buffer, strlen(buffer)-1, 0, (struct sockaddr *) &dest_addr, sizeof(dest_addr));
        int num = 0;
        if((num = recvfrom(sockfd, buffer, 99, 0, (struct sockaddr*) &dest_addr, &len)) < 0) {
            printf("Error in receiving message...\n");
        }
        buffer[num] = '\0'; 
        printf("Packet from IP: %s PORT: %d Msg: %s\n", inet_ntoa(dest_addr.sin_addr), ntohs(dest_addr.sin_port), buffer);
        memset(buffer,0,256);
    }
}
