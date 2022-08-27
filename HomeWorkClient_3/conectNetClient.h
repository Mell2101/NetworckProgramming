#ifndef CONECTNETCLIENT_H
#define CONECTNETCLIENT_H

#include <cstring>
#include <socketClient.h>
#include <unistd.h>

#define SA struct sockaddr

class ConectNet{
    SocketClient _sc;
public:

    ssize_t writen(int fd, const char *buffer, size_t n);
    void str_cli(FILE* fp, int sockfd);
    void conectN();
};

ssize_t ConectNet::writen(int fd, const char *buffer, size_t n){
    ssize_t numWritten; /* # of bytes written by last write() */
    size_t totWritten; /* Total # of bytes written so far */
    const char *buf;

    buf = buffer; /* No pointer arithmetic on "void *" */
    for (totWritten = 0; totWritten < n; ) {
    numWritten = write(fd, buf, n - totWritten);

    /* The "write() returns 0" case should never happen, but the
    following ensures that we don't loop forever if it does */

    if (numWritten <= 0) {
    if (numWritten == -1 && errno == EINTR)
    continue; /* Interrupted —> restart write() */
    else
    return -1; /* Some other error */
    }
    totWritten += numWritten;
    buf += numWritten;
    }
    return totWritten; /* Must be 'n' bytes if we get here */
}

void ConectNet::str_cli(FILE *fp, int sockfd){
    char sendLine[4096], recvline[4096];

    while(fgets(sendLine, 4096, fp) != NULL){
        writen(sockfd, sendLine, std::strlen(sendLine));

        fputs(recvline, stdout);
    }
}

void ConectNet::conectN(){
    int sockfd = _sc.getSockfd();
    connect(sockfd, (SA*)& _sc.servaddr, sizeof (_sc.servaddr));
    str_cli(stdin, sockfd);
}

#endif // CONECTNETCLIENT_H
