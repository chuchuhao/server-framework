#ifndef HTTP_PARSER_H
#define http_PARSER_H

#define DATA_BUF_SIZE 1024

#include <stdlib.h>

struct header_node {
    char *item;
    struct header_node *next;
};

struct http_parser {
    struct header_node *header;
    char data[DATA_BUF_SIZE];
};

typedef struct header_node header_node;
typedef struct http_parser http_parser;

header_node *new_node(const char *item);

size_t http_parser_execute(http_parser *parser,
                           char *data,
                           size_t len);

http_parser* http_parser_init(void);

void http_parser_print(http_parser *parser);

void http_parser_free(http_parser *parser);

#endif

