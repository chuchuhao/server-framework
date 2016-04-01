#include "buffer.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

ssize_t writing_hook(server_pt srv, int fd, void *data, size_t len)
{
    char prefix[5] = "hook-";
    size_t totalLen = len + strlen(prefix) + 1;
    char *encryptData = malloc( totalLen );
    strcpy( encryptData, prefix);
    strcat( encryptData, data);
    int sent = write(fd, encryptData, totalLen);
    if (sent < 0 && (errno & (EWOULDBLOCK | EAGAIN | EINTR)))
        sent = 0;
    return sent;
}


int main(void)
{
    fprintf(stderr, "# Test buffer\n");

    /* simple check buffer */
    void *buf = Buffer.new(0);
    char strA[12] = "hello world\n";
    char strB[12] = "good byyyye\n";

    if ( Buffer.write( buf, strA, strlen(strA)) < 0 )
        perror("write error");

    if ( Buffer.write( buf, strB, strlen(strB)) < 0)
        perror("write_move error");

    Buffer.set_whook( buf, writing_hook);

    while( !Buffer.is_empty( buf) ) {
        if ( Buffer.flush( buf, STDOUT_FILENO) < 0 ) {
            perror("flush failed");
        }
    }

    return 0;
}

