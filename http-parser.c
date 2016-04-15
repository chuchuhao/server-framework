#include "http-parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

header_node *new_node(const char *item)
{
    header_node *dect = malloc(sizeof(header_node));
    dect->item = strdup(item);
    return dect;
}
http_parser* http_parser_init(void)
{
    http_parser *parser = malloc(sizeof(struct http_parser));
    if(!parser) return 0;

    *parser = (struct http_parser) {
        .header = NULL,
         .data = {0},
    };

    return parser;
};

void http_parser_free(http_parser *parser)
{
    header_node *head;
    while(parser->header != NULL ) {
        head = parser->header;
        parser->header = parser->header->next;
        free(head);
    }
    return ;
}

void http_parser_print(http_parser *parser)
{
    printf("Header -- \n");
    header_node *travel = parser->header;
    while(travel) {
        printf("item  : %s\n", travel->item);
        travel = travel->next;
    }
    printf("data   -- \n");
    printf("<start>%s<end>\n", parser->data);
    return ;
}


size_t http_parser_execute(http_parser *parser,
                           char *data,
                           size_t len)
{
    char *line = strtok(data, "\r\n");
    while( line &&  line != "" ) {
        header_node *newHead = new_node(line);
        newHead->next = parser->header;
        parser->header = newHead;
        line = strtok(NULL, "\r\n");
    };
    return 1;
}

