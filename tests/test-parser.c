/* simple HTTP server parsing header information */

#include "http-parser.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

/* accep connections, for HTTP keep-alive */

static int srvfd;


int main(void)
{
    printf("Serving HTTP on 127.0.0.1 port 8080\n"
           "now open up the URL http://localhost:8080 on your browser\n\n");

    /* create a server socker */
    char *port = "8080";
    struct addrinfo hints;
    struct addrinfo *servinfo;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &hints, &servinfo)) {
        perror("address error");
        return -1;
    }

    srvfd = socket(servinfo->ai_family, servinfo->ai_socktype,
                   servinfo->ai_protocol);
    {
        int optval = 1; /* prevent from address begin taken */
        setsockopt(srvfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    }

    /* bind the address to the socket*/
    if (bind(srvfd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
        perror("bind error");
        return -1;
    }

    /* listen incoming */
    listen(srvfd, SOMAXCONN);
    if (errno)
        perror("starting. last error was");

    freeaddrinfo(servinfo);

    while (1) {
        int clifd = 0;
        // TODO: repaire this
        struct sockaddr_in clinfo;
        socklen_t addrlen = sizeof (clinfo);
        clifd = accept(srvfd, (struct sockaddr *)&clinfo,
                       &addrlen);
        char buff[1024];
        ssize_t len;
        static char response[] =
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: 12\r\n"
            "Connection: keep-alive\r\n"
            "keep-Alive: timeout=2\r\n"
            "\r\n"
            "Hello World!\r\n";
        if ((len = recv(clifd, buff, sizeof(buff), 0)) > 0) {
            fprintf(stderr, "receieve simething\n");
            /* Parse */
            printf("%zu\n", len);
            printf("%s\n", buff);

            fprintf(stderr, "testing buffer -------------------\n");
            http_parser *parser = http_parser_init();
            if( http_parser_execute(parser, buff, len) <= 0 ) {
                fprintf(stderr, "no information\n");
            } else {
                http_parser_print(parser);
            }
            http_parser_free(parser);
            fprintf(stderr, "----------------------------------\n");

            len = write(clifd, response, strlen(response));
        }
    }

    if (errno) {
        perror("\nFinished. last error was");
        return -1;
    }
    return 1;
}
