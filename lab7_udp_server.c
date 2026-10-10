/* Roll: 24636 Service Type: A d = 0 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ROLL "24636"
#define PORT 30636

int main() {
    int s;
    char b[1024], r[1024];
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        perror("socket");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (struct sockaddr *)&server,
             sizeof(server)) < 0) {
        perror("bind");
        close(s);
        return 1;
    }

    printf("Server running on port %d\n", PORT);

    while (1) {
        int n = recvfrom(s, b, sizeof(b) - 1, 0,
                         (struct sockaddr *)&client, &len);
        if (n < 0) continue;
        b[n] = '\0';

        char *p = strchr(b, '#');

        if (!p || (size_t)(p - b) != strlen(ROLL) ||
            strncmp(b, ROLL, strlen(ROLL)) != 0) {
            strcpy(r, "ERROR: Unauthorized client");
        } else {
            char *m = p + 1;

            printf("Received: %s\n", m);

            if (strcmp(m, "EXIT") == 0) {
                strcpy(r, "Server shutting down");
                sendto(s, r, strlen(r), 0,
                       (struct sockaddr *)&client, len);
                break;
            }

            char *t = strchr(m, '|');

            if (!t) {
                strcpy(r, "ERROR: Invalid command");
            } else {
                *t++ = '\0';

                if (strcmp(m, "UP") == 0) {
                    for (int i = 0; t[i]; i++)
                        t[i] = toupper((unsigned char)t[i]);
                    snprintf(r, sizeof(r), "%s", t);
                } else if (strcmp(m, "LOW") == 0) {
                    for (int i = 0; t[i]; i++)
                        t[i] = tolower((unsigned char)t[i]);
                    snprintf(r, sizeof(r), "%s", t);
                } else if (strcmp(m, "REV") == 0) {
                    int k = strlen(t);
                    for (int i = 0; i < k; i++)
                        r[i] = t[k - 1 - i];
                    r[k] = '\0';
                } else {
                    strcpy(r, "ERROR: Invalid command");
                }
            }
        }

        sendto(s, r, strlen(r), 0,
               (struct sockaddr *)&client, len);
        printf("Response: %s\n", r);
    }

    close(s);
    printf("Server stopped.\n");
    return 0;
}
