#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_


#define ERROR_404 "html_doc/error_404.html"
#define ERROR_400 "html_doc/error_400.html"

//#define DEBUG
#ifdef DEBUG
#define MY_PRINTF(a, b) printf("%s\n", b) 
#else
#define MY_PRINTF(a, b)
#endif

int startServer(const char * host, int port);
static char* find_page(char *p, size_t size);
const char *parse_http_request(char *buf, int sockfd);

#endif
