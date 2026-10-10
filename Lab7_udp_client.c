/* Roll: 24636 - UDP Client */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    int s, port;
    char b[1024], msg[1100], r[1024];
    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    if (argc != 3) {
        printf("Usage: %s <server_ip> <roll>\n", argv[0]);
        return 1;
    }

    int nroll = atoi(argv[2]);
    char *last = argv[2] + strlen(argv[2]) -
                 (strlen(argv[2]) >= 3 ? 3 : strlen(argv[2]));
    port = 30000 + atoi(last);

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        perror("socket");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (inet_pton(AF_INET, argv[1],
                  &server.sin_addr) != 1) {
        printf("Invalid IP address\n");
        close(s);
        return 1;
    }

    (void)nroll;

    while (1) {
        printf("Enter command (UP|text, LOW|text, REV|text, EXIT): ");

        if (!fgets(b, sizeof(b), stdin))
            strcpy(b, "EXIT");

        b[strcspn(b, "\r\n")] = '\0';

        snprintf(msg, sizeof(msg), "%s#%s", argv[2], b);

        if (sendto(s, msg, strlen(msg), 0,
                   (struct sockaddr *)&server, len) < 0) {
            perror("sendto");
            break;
        }

        int n = recvfrom(s, r, sizeof(r) - 1, 0, NULL, NULL);
        if (n < 0) {
            perror("recvfrom");
            break;
        }

        r[n] = '\0';
        printf("Server response: %s\n", r);

        if (strcmp(b, "EXIT") == 0)
            break;
    }

    close(s);
    return 0;
}
