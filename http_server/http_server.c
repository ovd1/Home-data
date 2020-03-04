#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "http_server.h"

int startServer(const char * host, int port) {
    int sock;
    struct sockaddr_in addr;
    struct hostent * host_ent;
    char * hostAddr;
    
    int hport = 8080;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(hport);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {  
        printf("Error creating socket!\n");  
        exit(1);  
    }  
    printf("Socket created...\n");

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        printf("Error binding socket to port!\n");  
        exit(1);
    }
    printf("Binding done...\n");
    
    if (listen(sock, 100) == -1){
        perror("listen error");
        
        close(sock);
        exit(1);
    }
    
    return sock;
}

  
static char* find_page(char *p, size_t size){
    while (size-- != 0 && *(p++) != ' ');
    if (*(p - 1) == ' '){
        return (p - 1);
    } else {
        return NULL;
    }
    
}


const char *parse_http_request(char *buf, int sockfd){
    int  comp, read_size; 
    char *p, *result;

    if (sockfd != -1){
        read_size = read(sockfd, buf, 1024);

        if (read_size >= 0) {
            printf("%s\n", buf);
            p = buf;
            comp = memcmp(p, "GET", 3);
            if (comp == 0) {
                p += 3;
                if (*p == ' '){
                    p++;
                    result = p;
                    p = find_page(p, read_size - 4);
                    if (p != NULL) { 
                        comp = memcmp(p, " HTTP/1.1\r\n", sizeof(" HTTP/1.1\r\n") - 1);
                        *p = '\0';
                        if (!comp)
                            return result;
                    }
                }
            }
        }
    }
    
    return NULL;
} 
