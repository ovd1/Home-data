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

#include "../uart.h"

#include "http_server.c"
#include "http_server.h"

static const char response_data[] = "HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n";


static const char response_header_200Ok[] = "HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n";

static const char resp_404[] = "HTTP/1.1 404 Not Found\r\n"
"Server: Apache/2.2.14 (Win32)\r\n"
"Connection: close\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n";

static const char resp_400[] = "HTTP/1.1 400 Bad Request\r\n"
"Server: Apache/2.2.14 (Win32)\r\n"
"Connection: close\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n";


int main()
{
    int sockfd, comp, port; 
    static char buf[1024], buf2[1024] = "html_doc"; 
    const char *host;
    
    sockfd = startServer(host, port);
 
    while (1){
        const char *page_name_begin;
        int open_file, bytes_read, err_file;  
        int sock2 = accept(sockfd, NULL, NULL);
        
        if (sock2 < 0) {
            continue;
        }
        
        
        page_name_begin = parse_http_request(buf, sock2);
        //printf("result %s\n", page_name_begin);
        MY_PRINTF("result", page_name_begin); 
        
        
        if (page_name_begin) {
            buf2[sizeof("html_doc") - 1] = '\0';
            strncat(buf2, page_name_begin, sizeof(buf2));
            buf2[1023] = '\0';
            MY_PRINTF("BUF=%s\n", buf2);
            
            if (strcmp(buf2+sizeof("html_doc"), "data") == 0)
            {
                int data_read = get_temp_humid(buf, 8);
                buf[8] = '\0'; 
                
                if (data_read > 0) 
                {
                    write(sock2, response_header_200Ok, sizeof(response_header_200Ok)-1);
                    write(sock2, buf, data_read);
                    write(sock2, "\r\n", sizeof("\r\n")-1);
                    
                }
            }
                        
            else if (strcmp(buf2+sizeof("html_doc"), "temperature/") == 0)
            {
                int data_read = get_temp_humid(buf, 8); 
                buf[8] = '\0'; 
                if (data_read > 0) 
                {
                    write(sock2, response_header_200Ok, sizeof(response_header_200Ok)-1);
                    printf("\nttt=%s zzz=%s\n", &buf[6], buf);
                    write(sock2, &buf[6], 2);
                    write(sock2, "\r\n", sizeof("\r\n")-1);
                    
                }
            }
            else if (strcmp(buf2+sizeof("html_doc"), "humidity") == 0)
            {
                int data_read = get_temp_humid(buf, 8); 
                buf[8] = '\0'; 
                if (data_read > 0) 
                {
                    write(sock2, response_header_200Ok, sizeof(response_header_200Ok)-1);
                    printf("\nttt=%s zzz=%s\n", &buf[2], buf);
                    write(sock2, &buf[2], 2);
                    write(sock2, "\r\n", sizeof("\r\n")-1);
                    
                }
            }
            else {
                open_file = open(buf2, 0, O_RDONLY);
                //comp = memcmp(page_name_begin, "/index.htm", sizeof("/index.htm") - 1);
                if (open_file != -1){
                    write(sock2, response_header_200Ok, sizeof(response_header_200Ok)-1);
                    
                    while ((bytes_read = read(open_file, buf, sizeof(buf))) > 0) {
                        write(sock2, buf, bytes_read);
                    }
                    write(sock2, "\r\n", sizeof("\r\n")-1);
                } else {
                    err_file = open(ERROR_404, 0, O_RDONLY);
                    write(sock2, resp_404, sizeof(resp_404)-1);
                    bytes_read = read(err_file, buf, sizeof(buf));
                    
                    write(sock2, buf, bytes_read);
                    write(sock2, "\r\n", sizeof("\r\n")-1);
                }
            }    
        } else {
            //TODO: 401 response
            err_file = open(ERROR_400, 0, O_RDONLY);
            write(sock2, resp_400, sizeof(resp_400)-1);
            bytes_read = read(err_file, buf, sizeof(buf));
                
            write(sock2, buf, bytes_read);
            write(sock2, "\r\n", sizeof("\r\n")-1);
        }
        
        close(sock2);
    }


    close(sockfd);
    return 0;
}
